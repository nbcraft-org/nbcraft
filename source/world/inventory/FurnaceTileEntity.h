// FurnaceTileEntity.h
#ifndef NBCRAFT_FURNACETILEENTITY_H
#define NBCRAFT_FURNACETILEENTITY_H

#include "world/Container.hpp"
#include "world/item/ItemStack.hpp"
#include "world/level/tileentity/TileEntity.h"

class FurnaceTileEntity : public TileEntity, public Container {
private:
    ItemStack m_items[3];

public:
    int furnaceBurnTime;
    int currentItemBurnTime;
    int furnaceCookTime;

    FurnaceTileEntity(const TilePos& pos)
        : TileEntity("Furnace", pos),
          furnaceBurnTime(0), currentItemBurnTime(0), furnaceCookTime(0)
    {
        m_items[0] = ItemStack::EMPTY;
        m_items[1] = ItemStack::EMPTY;
        m_items[2] = ItemStack::EMPTY;
    }

    ~FurnaceTileEntity() {}

    // --- Container Overrides ---
    uint16_t getContainerSize() const override;
    ItemStack& getItem(int index) override;
    void setItem(int index, const ItemStack& item) override;
    ItemStack removeItem(int index, int count) override;
    std::string getName() const override;
    int getMaxStackSize() override;
    void setChanged() override;
    bool stillValid(Player* player) const override;

    // --- TileEntity Overrides ---
    void save(CompoundTag& tag) const override;
    void load(const CompoundTag& tag) override;

    // --- Furnace Logic ---
    void tick();
    bool isBurning() const;
    bool canSmelt();
    void smeltItem();
    int getBurnDuration(const ItemStack& item);
};

#endif // NBCRAFT_FURNACETILEENTITY_H