#include "FurnaceTileEntity.h"
#include "world/item/crafting/FurnaceRecipes.hpp"
#include "world/item/Item.hpp"
#include "world/item/TileItem.hpp"
#include "world/tile/FurnaceTile.hpp"
#include "world/level/Level.hpp"
#include "common/Utils.hpp"

void FurnaceTileEntity::save(CompoundTag& tag) const {
    TileEntity::save(tag); // writes id, x, y, z
    tag.putInt32("BurnTime", furnaceBurnTime);
    tag.putInt32("CookTime", furnaceCookTime);
    tag.putInt32("BurnTimeTotal", currentItemBurnTime);

    ListTag* items = new ListTag();
    for (int i = 0; i < 3; i++) {
        if (!m_items[i].isEmpty()) {
            CompoundTag* slot = new CompoundTag();
            slot->putInt8("Slot", (int8_t)i);
            m_items[i].save(*slot);
            items->add(slot);
        }
    }
    tag.put("Items", items);
}

void FurnaceTileEntity::load(const CompoundTag& tag) {
    TileEntity::load(tag); // reads x, y, z
    furnaceBurnTime    = tag.getInt32("BurnTime");
    furnaceCookTime    = tag.getInt32("CookTime");
    currentItemBurnTime = tag.getInt32("BurnTimeTotal");

    const ListTag* items = tag.getList("Items");
    if (items) {
        for (unsigned int i = 0; i < items->rawView().size(); i++) {
            const CompoundTag* slot = items->getCompound(i);
            if (!slot) continue;
            int slotIdx = (uint8_t)slot->getInt8("Slot");
            if (slotIdx >= 0 && slotIdx < 3)
                m_items[slotIdx] = ItemStack::fromTag(*slot);
        }
    }
}

uint16_t FurnaceTileEntity::getContainerSize() const { return 3; }

ItemStack& FurnaceTileEntity::getItem(int index) { return m_items[index]; }

void FurnaceTileEntity::setItem(int index, const ItemStack& item) {
    m_items[index] = item;
    if (item.m_count > getMaxStackSize()) {
        m_items[index].m_count = getMaxStackSize();
    }
    setChanged();
}

ItemStack FurnaceTileEntity::removeItem(int index, int count) {
    if (!m_items[index].isEmpty()) {
        if (m_items[index].m_count <= count) {
            ItemStack result = m_items[index];
            m_items[index] = ItemStack::EMPTY;
            setChanged();
            return result;
        }
        ItemStack result = m_items[index];
        result.m_count = count;
        m_items[index].m_count -= count;
        setChanged();
        return result;
    }
    return ItemStack::EMPTY;
}

std::string FurnaceTileEntity::getName() const { return "Furnace"; }

int FurnaceTileEntity::getMaxStackSize() { return 64; }

void FurnaceTileEntity::setChanged() { markDirty(); }

bool FurnaceTileEntity::stillValid(Player* player) const { return true; }

bool FurnaceTileEntity::isBurning() const { return furnaceBurnTime > 0; }

void FurnaceTileEntity::tick() {
    bool wasBurning = isBurning();
    bool needsUpdate = false;

    // 1. Burn fuel
    if (furnaceBurnTime > 0) {
        furnaceBurnTime--;
    }

    // 2. Check if we need to consume more fuel to keep smelting
    if (furnaceBurnTime == 0 && canSmelt()) {
        currentItemBurnTime = furnaceBurnTime = getBurnDuration(m_items[1]);
        if (furnaceBurnTime > 0) {
            needsUpdate = true;
            if (!m_items[1].isEmpty()) {
                m_items[1].m_count--;
                if (m_items[1].m_count == 0) m_items[1] = ItemStack::EMPTY;
            }
        }
    }

    // 3. Process the smelting item
    if (isBurning() && canSmelt()) {
        furnaceCookTime++;
        if (furnaceCookTime == 200) { // 200 ticks = 10 seconds per item
            furnaceCookTime = 0;
            smeltItem();
            needsUpdate = true;
        }
    } else {
        furnaceCookTime = 0;
    }

    if (wasBurning != isBurning()) {
        TileData currentData = level->getData(pos);
        FurnaceTile::s_swappingLitState = true;
        level->setTileAndData(pos, isBurning() ? TILE_FURNACE_LIT : TILE_FURNACE, currentData);
        FurnaceTile::s_swappingLitState = false;
        needsUpdate = true;
    }

    if (needsUpdate) {
        setChanged();
    }
}

bool FurnaceTileEntity::canSmelt() {
    if (m_items[0].isEmpty()) return false;
    
    // We pass 'this' because the recipe manager wants the whole container to check slot 0
    ItemStack result = FurnaceRecipes::singleton().getItemFor(this);
    if (result.isEmpty()) return false;

    // Check if output slot is empty, matches the result type, and has room
    if (m_items[2].isEmpty()) return true;

    // We use getId() instead of trying to access a private variable
    if (m_items[2].getId() != result.getId()) return false;

    if (m_items[2].m_count < getMaxStackSize() && m_items[2].m_count < m_items[2].getMaxStackSize()) return true;

    return m_items[2].m_count < result.getMaxStackSize();
}

void FurnaceTileEntity::smeltItem() {
    if (!canSmelt()) return;

    ItemStack result = FurnaceRecipes::singleton().getItemFor(this);

    if (m_items[2].isEmpty()) {
        m_items[2] = result;
    } else if (m_items[2].getId() == result.getId()) {
        m_items[2].m_count += result.m_count; // Add to existing stack
    }

    // Consume the raw material
    m_items[0].m_count--;
    if (m_items[0].m_count <= 0) m_items[0] = ItemStack::EMPTY;
}

int FurnaceTileEntity::getBurnDuration(const ItemStack& item) {
    // We completely delete the hardcoded items and just ask the recipe manager!
    return FurnaceRecipes::singleton().getBurnDuration(item);
}