#include "CreativeMenu.hpp"
#include "Slot.hpp"
#include "common/Utils.hpp"
#include "world/tile/Tile.hpp"
#include "world/item/Item.hpp"

CreativeMenu::CreativeMenu(Container* inventory, Container* container)
    : ContainerMenu(Container::CONTAINER), m_container(container)
{
#if defined(_DEBUG)
    // Get every tile by ID
    for (int id = 1; id < C_MAX_TILES; id++)
    {
        Tile* tile = Tile::tiles[id];
        if (!tile) continue;

        if (id == TILE_CLOTH) continue;

        if (id == TILE_TREE_TRUNK)
        {
            for (int aux = 0; aux < 3; aux++)
                m_creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else if (id == TILE_STONESLAB_HALF)
        {
            for (int aux = 0; aux < 4; aux++)
                m_creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else if (id == TILE_SAPLING)
        {
            for (int aux = 0; aux < 3; aux++)
                m_creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else
        {
            m_creativeItems.push_back(ItemStack(tile, 1, 0));
        }
    }
#else
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_STONEBRICK], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_STONE], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_ORE_EMERALD], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_ORE_GOLD], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_ORE_IRON], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_ORE_COAL], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_ORE_LAPIS], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLAY], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_BLOCK_EMERALD], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_BLOCK_GOLD], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_BLOCK_IRON], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_BEDROCK], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_BLOCK_LAPIS], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_BRICKS], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_MOSS_STONE], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_STONESLAB_HALF], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_STONESLAB_HALF], 1, 1));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_STONESLAB_HALF], 1, 2));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_STONESLAB_HALF], 1, 3));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_OBSIDIAN], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_NETHERRACK], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_SOUL_SAND], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_GLOWSTONE], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_TREE_TRUNK], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_TREE_TRUNK], 1, 1));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_TREE_TRUNK], 1, 2));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_LEAVES], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_LEAVES], 1, 1));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_LEAVES], 1, 2));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_DIRT], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_GRASS], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_SAND], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_SANDSTONE], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_GRAVEL], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_COBWEB], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_WOOD], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_SAPLING], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_SAPLING], 1, 1));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_SAPLING], 1, 2));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_DEAD_BUSH], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_SPONGE], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_ICE], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_SNOW], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_FLOWER], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_ROSE], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_MUSHROOM_1], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_MUSHROOM_2], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_REEDS], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CACTUS], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_PUMPKIN], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_PUMPKIN_LIT], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_TALL_GRASS], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CHEST], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_WORKBENCH], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_GLASS], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_TNT], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_BOOKSHELF], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 1));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 2));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 3));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 4));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 5));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 6));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 7));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 8));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 9));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 10));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 11));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 12));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 13));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 14));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CLOTH], 1, 15));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_DISPENSER], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_FURNACE], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_NOTE_BLOCK], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_JUKEBOX], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_PISTON_STICKY], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_PISTON], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_FENCE], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_FENCE_GATE], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_LADDER], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_RAIL], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_RAIL_POWERED], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_RAIL_ACTIVATOR], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_TORCH], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_STAIRS_WOOD], 1, 0));
    m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_STAIRS_STONE], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_LEVER], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_PLATE_STONE], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_PLATE_WOOD], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_NOT_GATE_ON], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_BUTTON_STONE], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_CAKE], 1, 0));
    //m_creativeItems.push_back(ItemStack(Tile::tiles[TILE_TRAPDOOR], 1, 0));
#endif
    for (int id = 256; id < C_MAX_ITEMS; id++)
    {
        Item* item = Item::items[id];
        if (!item) continue;
        m_creativeItems.push_back(ItemStack(item, 1, 0));
    }

    Item* dye = Item::items[ITEM_DYE_POWDER];
    if (dye)
    {
        for (int aux = 1; aux < 16; aux++)
            m_creativeItems.push_back(ItemStack(dye, 1, aux));
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
    int scrollLimit = m_creativeItems.size() / 8 - 8 + 1;
    if (scrollLimit < 0) scrollLimit = 0;
    int scrollOffset = (int)((scroll * scrollLimit) + 0.5f);

    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            int itemIndex = col + (row + scrollOffset) * 8;
            if (itemIndex >= 0 && itemIndex < (int)m_creativeItems.size())
            {
                m_container->setItem(col + row * 8, m_creativeItems[itemIndex]);
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
