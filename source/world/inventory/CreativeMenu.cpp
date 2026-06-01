#include "CreativeMenu.hpp"
#include "Slot.hpp"
#include "common/Utils.hpp"
#include "world/tile/Tile.hpp"
#include "world/item/Item.hpp"

CreativeMenu::CreativeMenu(Container* inventory, Container* container)
    : ContainerMenu(Container::CONTAINER), m_container(container)
{
    // Get every tile and item by ID
    for (int id = 1; id < C_MAX_TILES; id++)
    {
        Tile* tile = Tile::tiles[id];
        if (!tile) continue;

        if (id == TILE_CLOTH) continue;

        if (id == TILE_TREE_TRUNK)
        {
            for (int aux = 0; aux < 3; aux++)
                creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else if (id == TILE_STONESLAB_HALF)
        {
            for (int aux = 0; aux < 4; aux++)
                creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else if (id == TILE_SAPLING)
        {
            for (int aux = 0; aux < 3; aux++)
                creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else
        {
            creativeItems.push_back(ItemStack(tile, 1, 0));
        }
    }

    for (int id = 256; id < C_MAX_ITEMS; id++)
    {
        Item* item = Item::items[id];
        if (!item) continue;
        creativeItems.push_back(ItemStack(item, 1, 0));
    }

    Item* dye = Item::items[ITEM_DYE_POWDER];
    if (dye)
    {
        for (int aux = 1; aux < 16; aux++)
            creativeItems.push_back(ItemStack(dye, 1, aux));
    }

    int rows = m_container->getContainerSize() / 8;
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            int index = col + row * 8;
            addSlot(new Slot(m_container, index));
        }
    }

    for (int col = 0; col < 9; ++col)
    {
        addSlot(new Slot(inventory, col, Slot::HOTBAR));
    }

    updateScroll(0.0f);
}

void CreativeMenu::updateScroll(float scroll)
{
    int scrollLimit = creativeItems.size() / 8 - 8 + 1;
    if (scrollLimit < 0) scrollLimit = 0;
    int scrollOffset = (int)((scroll * scrollLimit) + 0.5f);

    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            int itemIndex = col + (row + scrollOffset) * 8;
            if (itemIndex >= 0 && itemIndex < (int)creativeItems.size())
            {
                m_container->setItem(col + row * 8, creativeItems[itemIndex]);
            }
            else
            {
                m_container->setItem(col + row * 8, ItemStack());
            }
        }
    }
}

bool CreativeMenu::stillValid(Player* player) const
{
    return true;
}
