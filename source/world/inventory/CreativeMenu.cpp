#include "CreativeMenu.hpp"
#include "Slot.hpp"
#include "common/Utils.hpp"
#include "world/tile/Tile.hpp"
#include "world/item/Item.hpp"

std::vector<ItemStack> CreativeMenu::s_creativeItems;
bool CreativeMenu::s_initialized = false;
#if !defined(_DEBUG)
static const eTileID creativeTiles[] =
{
    TILE_STONEBRICK,
    TILE_STONE,
    TILE_ORE_EMERALD,
    TILE_ORE_GOLD,
    TILE_ORE_IRON,
    TILE_ORE_COAL,
    TILE_ORE_LAPIS,
    TILE_CLAY,
    TILE_BLOCK_EMERALD,
    TILE_BLOCK_GOLD,
    TILE_BLOCK_IRON,
    TILE_BEDROCK,
    TILE_BLOCK_LAPIS,
    TILE_BRICKS,
    TILE_MOSS_STONE,
    TILE_STONESLAB_HALF,
    TILE_OBSIDIAN,
    TILE_OBSIDIAN_CRYING,
    TILE_NETHERRACK,
    TILE_SOUL_SAND,
    TILE_GLOWSTONE,
    TILE_TREE_TRUNK,
    TILE_LEAVES,
    TILE_DIRT,
    TILE_GRASS,
    TILE_SAND,
    TILE_SANDSTONE,
    TILE_GRAVEL,
    TILE_COBWEB,
    TILE_WOOD,
    TILE_SAPLING,
    TILE_DEAD_BUSH,
    TILE_SPONGE,
    TILE_ICE,
    TILE_SNOW,
    TILE_FLOWER,
    TILE_ROSE,
    TILE_MUSHROOM_1,
    TILE_MUSHROOM_2,
    TILE_REEDS,
    TILE_CACTUS,
    TILE_PUMPKIN,
    TILE_PUMPKIN_LIT,
    TILE_TALL_GRASS,
    TILE_CHEST,
    TILE_WORKBENCH,
    TILE_GLASS,
    TILE_TNT,
    TILE_BOOKSHELF,
    TILE_CLOTH,
    //TILE_DISPENSER,
    TILE_FURNACE,
    //TILE_JUKEBOX,
    //TILE_PISTON_STICKY,
    //TILE_PISTON,
    TILE_FENCE,
    //TILE_FENCE_GATE,
    TILE_LADDER,
    TILE_RAIL,
    TILE_RAIL_POWERED,
    TILE_RAIL_ACTIVATOR,
    TILE_TORCH,
    TILE_STAIRS_WOOD,
    TILE_STAIRS_STONE,
    //TILE_LEVER,
    //TILE_PLATE_STONE,
    //TILE_PLATE_WOOD,
    //TILE_NOT_GATE_ON,
    //TILE_BUTTON_STONE,
    //TILE_CAKE,
    //TILE_TRAPDOOR,
    TILE_ROCKET_LAUNCHER,
};
#endif
void CreativeMenu::initCreativeItems()
{
    if (s_initialized) return;
    s_initialized = true;
#if defined(_DEBUG)
    for (int id = 1; id < C_MAX_TILES; id++)
    {
        Tile* tile = Tile::tiles[id];
        if (!tile) continue;

        if (id == TILE_CLOTH) continue;

        if (id == TILE_TREE_TRUNK)
        {
            for (int aux = 0; aux < 3; aux++)
                s_creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else if (id == TILE_STONESLAB_HALF)
        {
            for (int aux = 0; aux < 4; aux++)
                s_creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else if (id == TILE_SAPLING)
        {
            for (int aux = 0; aux < 3; aux++)
                s_creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else
        {
            s_creativeItems.push_back(ItemStack(tile, 1, 0));
        }
    }
#else
    for (int i = 0; i < sizeof(creativeTiles) / sizeof(creativeTiles[0]); i++)
    {
        eTileID tileId = creativeTiles[i];
        Tile* tile = Tile::tiles[tileId];
        if (!tile) continue;

        if (tileId == TILE_CLOTH)
        {
            for (int aux = 0; aux < 16; aux++)
                s_creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else if (tileId == TILE_STONESLAB_HALF)
        {
            for (int aux = 0; aux < 4; aux++)
                s_creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else if (tileId == TILE_TREE_TRUNK)
        {
            for (int aux = 0; aux < 3; aux++)
                s_creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else if (tileId == TILE_LEAVES)
        {
            for (int aux = 0; aux < 3; aux++)
                s_creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else if (tileId == TILE_SAPLING)
        {
            for (int aux = 0; aux < 3; aux++)
                s_creativeItems.push_back(ItemStack(tile, 1, aux));
        }
        else
        {
            s_creativeItems.push_back(ItemStack(tile, 1, 0));
        }
    }
#endif
    for (int id = 256; id < C_MAX_ITEMS; id++)
    {
        Item* item = Item::items[id];
        if (!item) continue;
        s_creativeItems.push_back(ItemStack(item, 1, 0));
    }

    Item* dye = Item::items[ITEM_DYE_POWDER];
    if (dye)
    {
        for (int aux = 1; aux < 16; aux++)
            s_creativeItems.push_back(ItemStack(dye, 1, aux));
    }
}

CreativeMenu::CreativeMenu(Container* inventory, Container* container)
    : ContainerMenu(Container::CONTAINER), m_container(container)
{
    initCreativeItems();

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
    int scrollLimit = s_creativeItems.size() / 8 - 8 + 1;
    if (scrollLimit < 0) scrollLimit = 0;
    int scrollOffset = (int)((scroll * scrollLimit) + 0.5f);

    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            int itemIndex = col + (row + scrollOffset) * 8;
            if (itemIndex >= 0 && itemIndex < (int)s_creativeItems.size())
            {
                m_container->setItem(col + row * 8, s_creativeItems[itemIndex]);
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
