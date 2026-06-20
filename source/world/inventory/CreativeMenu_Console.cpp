#include "CreativeMenu_Console.hpp"
#include "Slot.hpp"
#include "common/Utils.hpp"
#include "world/tile/Tile.hpp"
#include "world/item/Item.hpp"

std::vector<ItemStack> CreativeMenu_Console::creativeItems[];
bool CreativeMenu_Console::initialized = false;

void CreativeMenu_Console::initCreativeItems()
{
    if (initialized) return;
    initialized = true;

    _addTile(STRUCTURES, Tile::rock);
    _addTile(STRUCTURES, Tile::grass);
    _addTile(STRUCTURES, Tile::dirt);
    _addTile(STRUCTURES, Tile::stoneBrick);
    _addTile(STRUCTURES, Tile::sand);
    _addTile(STRUCTURES, Tile::sandStone);
    _addTile(STRUCTURES, Tile::goldBlock);
    _addTile(STRUCTURES, Tile::ironBlock);
    _addTile(STRUCTURES, Tile::lapisBlock);
    _addTile(STRUCTURES, Tile::emeraldBlock);
    _addTile(STRUCTURES, Tile::coalOre);
    _addTile(STRUCTURES, Tile::lapisOre);
    _addTile(STRUCTURES, Tile::emeraldOre);
    _addTile(STRUCTURES, Tile::redStoneOre);
    _addTile(STRUCTURES, Tile::ironOre);
    _addTile(STRUCTURES, Tile::goldOre);
    _addAuxTile(STRUCTURES, Tile::stoneSlabHalf);
    _addTile(STRUCTURES, Tile::unbreakable);
    _addTile(STRUCTURES, Tile::wood);
    _addAuxTile(STRUCTURES, Tile::treeTrunk);
    _addTile(STRUCTURES, Tile::gravel);
    _addTile(STRUCTURES, Tile::redBrick);
    _addTile(STRUCTURES, Tile::mossStone);
    _addTile(STRUCTURES, Tile::obsidian);
    _addTile(STRUCTURES, Tile::clay);
    _addTile(STRUCTURES, Tile::ice);
    _addTile(STRUCTURES, Tile::snow);
    _addTile(STRUCTURES, Tile::netherrack);
    _addTile(STRUCTURES, Tile::soulSand);
    _addTile(STRUCTURES, Tile::glowstone);
}

CreativeMenu_Console::CreativeMenu_Console(Container* inventory, Container* container)
    : ContainerMenu(Container::CONTAINER), m_container(container)
{
    initCreativeItems();

    int rows = m_container->getContainerSize() / 11;
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < 11; ++col)
        {
            int index = col + row * 11;
            addSlot(new Slot(m_container, index));
        }
    }

    for (int col = 0; col < 9; ++col)
    {
        addSlot(new Slot(inventory, col, Slot::HOTBAR));
    }

    updateGrid(0.0f);
}

void CreativeMenu_Console::_addAuxTile(Category tab, Tile* tile)
{
    int maxAux = 0;
    switch (tile->m_ID)
    {
    case TILE_CLOTH:          maxAux = 15; break;
    case TILE_STONESLAB_HALF: maxAux = 3;  break;
    case TILE_TREE_TRUNK:     maxAux = 2;  break;
    case TILE_LEAVES:         maxAux = 2;  break;
    case TILE_SAPLING:        maxAux = 2;  break;
    }
    for (int aux = 0; aux <= maxAux; aux++)
        creativeItems[tab].push_back(ItemStack(tile, 1, aux));
}

void CreativeMenu_Console::_addTile(Category tab, Tile* tile)
{
    creativeItems[tab].push_back(ItemStack(tile));
}

void CreativeMenu_Console::_addItem(Category tab, Item* item)
{
    creativeItems[tab].push_back(ItemStack(item));
}

void CreativeMenu_Console::updateGrid(float scroll, int tab)
{
    const std::vector<ItemStack>& creativeItems = GetCreativeItems(tab);
    int scrollLimit = creativeItems.size() / 55 - 55 + 1;
    if (scrollLimit < 0) scrollLimit = 0;
    int scrollOffset = (int)((scroll * scrollLimit) + 0.5f);
    int startSlot = 55 - (creativeItems.size() - scrollOffset * 55);

    for (int row = 0; row < 5; ++row)
    {
        for (int col = 0; col < 11; ++col)
        {
            const int slotIndex = col + row * 11;
            int itemIndex = slotIndex < startSlot ? -1 : col + row * 11 + scrollOffset - startSlot;
            if (itemIndex >= 0 && itemIndex < (int)creativeItems.size())
            {
                m_container->setItem(slotIndex, creativeItems[itemIndex]);
            }
            else
            {
                m_container->setItem(slotIndex, ItemStack::EMPTY);
            }
        }
    }
}

bool CreativeMenu_Console::stillValid(Player* player) const
{
    return true;
}
