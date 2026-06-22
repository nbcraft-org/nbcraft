#include "CreativeMenu_Console.hpp"
#include "Slot.hpp"
#include "common/Utils.hpp"
#include "world/tile/Tile.hpp"
#include "world/item/Item.hpp"
#include "world/item/DyeColor.hpp"
#include "world/tile/ClothTile.hpp"

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

    _addTile(DECORATION, Tile::sponge);
    //_addTile(DECORATION, Tile::melon);
    _addTile(DECORATION, Tile::pumpkin);
    _addTile(DECORATION, Tile::pumpkinLantern);
    _addAuxTile(DECORATION, Tile::sapling);
    _addAuxTile(DECORATION, Tile::leaves);
    //_addTile(DECORATION, Tile::vines);
    _addTile(DECORATION, Tile::torch);
    _addAuxTile(DECORATION, Tile::tallGrass);
    _addTile(DECORATION, Tile::flower);
    _addTile(DECORATION, Tile::rose);
    _addTile(DECORATION, Tile::mushroom1);
    _addTile(DECORATION, Tile::mushroom2);
    _addTile(DECORATION, Tile::cactus);
    _addTile(DECORATION, Tile::topSnow);
    _addTile(DECORATION, Tile::web);
    //_addTile(DECORATION, Tile::glassPane);
    _addTile(DECORATION, Tile::glass);
    _addItem(DECORATION, Item::painting);
    _addItem(DECORATION, Item::sign);
    _addTile(DECORATION, Tile::bookshelf);
    _addTile(DECORATION, Tile::cloth, ClothTile::RED);
    _addTile(DECORATION, Tile::cloth, ClothTile::ORANGE);
    _addTile(DECORATION, Tile::cloth, ClothTile::YELLOW);
    _addTile(DECORATION, Tile::cloth, ClothTile::LIME);
    _addTile(DECORATION, Tile::cloth, ClothTile::LIGHT_BLUE);
    _addTile(DECORATION, Tile::cloth, ClothTile::CYAN);
    _addTile(DECORATION, Tile::cloth, ClothTile::BLUE);
    _addTile(DECORATION, Tile::cloth, ClothTile::PURPLE);
    _addTile(DECORATION, Tile::cloth, ClothTile::MAGENTA);
    _addTile(DECORATION, Tile::cloth, ClothTile::PINK);
    _addTile(DECORATION, Tile::cloth, ClothTile::WHITE);
    _addTile(DECORATION, Tile::cloth, ClothTile::SILVER);
    _addTile(DECORATION, Tile::cloth, ClothTile::GRAY);
    _addTile(DECORATION, Tile::cloth, ClothTile::BLACK);
    _addTile(DECORATION, Tile::cloth, ClothTile::GREEN);
    _addTile(DECORATION, Tile::cloth, ClothTile::BROWN);

    //_addTile(REDSTONE_AND_TRANSPORT, Tile::dispenser);
    _addTile(REDSTONE_AND_TRANSPORT, Tile::musicBlock);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::piston);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::stickyPiston);
    _addTile(REDSTONE_AND_TRANSPORT, Tile::tnt);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::lever);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::button);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::stonePressurePlate);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::woodPressurePlate);
    _addItem(REDSTONE_AND_TRANSPORT, Item::redStone);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::repeater);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::rail);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::poweredRail);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::detectorRail);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::repeater);
    _addItem(REDSTONE_AND_TRANSPORT, Item::minecart);
    _addItem(REDSTONE_AND_TRANSPORT, Item::minecart_chest);
    _addItem(REDSTONE_AND_TRANSPORT, Item::minecart_furnace);
    _addItem(REDSTONE_AND_TRANSPORT, Item::saddle);
    _addItem(REDSTONE_AND_TRANSPORT, Item::boat);
    _addTile(REDSTONE_AND_TRANSPORT, Tile::stairs_wood);
    _addTile(REDSTONE_AND_TRANSPORT, Tile::stairs_stone);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::brickStairs);
    //_addTile(REDSTONE_AND_TRANSPORT, Tile::stoneBrickStairs);
    _addTile(REDSTONE_AND_TRANSPORT, Tile::ladder);
    _addTile(REDSTONE_AND_TRANSPORT, Tile::trapDoor);
    _addTile(REDSTONE_AND_TRANSPORT, Tile::fenceGate);
    _addItem(REDSTONE_AND_TRANSPORT, Item::door_wood);
    _addItem(REDSTONE_AND_TRANSPORT, Item::door_iron);

    _addItem(MATERIALS, Item::coal);
    _addItem(MATERIALS, Item::coal, 1);
    _addItem(MATERIALS, Item::emerald);
    _addItem(MATERIALS, Item::ironIngot);
    _addItem(MATERIALS, Item::goldIngot);
    _addItem(MATERIALS, Item::brick);
    _addItem(MATERIALS, Item::stick);
    _addItem(MATERIALS, Item::string);
    _addItem(MATERIALS, Item::feather);
    _addItem(MATERIALS, Item::flint);
    _addItem(MATERIALS, Item::leather);
    _addItem(MATERIALS, Item::sulphur);
    _addItem(MATERIALS, Item::clay);
    _addItem(MATERIALS, Item::yellowDust);
    _addItem(MATERIALS, Item::seeds);
    _addItem(MATERIALS, Item::wheat);
    _addItem(MATERIALS, Item::reeds);
    _addItem(MATERIALS, Item::egg);
    _addItem(MATERIALS, Item::sugar);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::RED);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::ORANGE);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::YELLOW);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::LIME);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::LIGHT_BLUE);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::CYAN);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::BLUE);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::PURPLE);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::MAGENTA);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::PINK);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::WHITE);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::SILVER);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::GRAY);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::BLACK);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::GREEN);
    _addItem(MATERIALS, Item::dye_powder, DyeColor::BROWN);

    _addItem(FOOD, Item::apple);
    _addItem(FOOD, Item::apple_gold);
    //_addItem(FOOD, Item::melonSlice);
    _addItem(FOOD, Item::mushroomStew);
    _addItem(FOOD, Item::bread);
    _addItem(FOOD, Item::cake);
    _addItem(FOOD, Item::fish_cooked);
    _addItem(FOOD, Item::fish_raw);
    _addItem(FOOD, Item::porkChop_cooked);
    _addItem(FOOD, Item::porkChop_raw);
    //_addItem(FOOD, Item::cookedBeef);
    //_addItem(FOOD, Item::beef);
    //_addItem(FOOD, Item::cookedChicken);
    //_addItem(FOOD, Item::chicken);
    //_addItem(FOOD, Item::rottenFlesh);

    _addItem(TOOLS, Item::helmet_cloth);
    _addItem(TOOLS, Item::chestplate_cloth);
    _addItem(TOOLS, Item::leggings_cloth);
    _addItem(TOOLS, Item::boots_cloth);
    _addItem(TOOLS, Item::sword_wood);
    _addItem(TOOLS, Item::shovel_wood);
    _addItem(TOOLS, Item::pickAxe_wood);
    _addItem(TOOLS, Item::hatchet_wood);
    _addItem(TOOLS, Item::hoe_wood);

    _addItem(TOOLS, Item::bow);
    _addItem(TOOLS, Item::arrow);

    _addItem(TOOLS, Item::helmet_chain);
    _addItem(TOOLS, Item::chestplate_chain);
    _addItem(TOOLS, Item::leggings_chain);
    _addItem(TOOLS, Item::boots_chain);
    _addItem(TOOLS, Item::sword_stone);
    _addItem(TOOLS, Item::shovel_stone);
    _addItem(TOOLS, Item::pickAxe_stone);
    _addItem(TOOLS, Item::hatchet_stone);
    _addItem(TOOLS, Item::hoe_stone);

    _addItem(TOOLS, Item::flintAndSteel);
    _addItem(TOOLS, Item::compass);

    _addItem(TOOLS, Item::helmet_iron);
    _addItem(TOOLS, Item::chestplate_iron);
    _addItem(TOOLS, Item::leggings_iron);
    _addItem(TOOLS, Item::boots_iron);
    _addItem(TOOLS, Item::sword_iron);
    _addItem(TOOLS, Item::shovel_iron);
    _addItem(TOOLS, Item::pickAxe_iron);
    _addItem(TOOLS, Item::hatchet_iron);
    _addItem(TOOLS, Item::hoe_iron);

    _addItem(TOOLS, Item::clock);
    _addItem(TOOLS, Item::shears);

    _addItem(TOOLS, Item::helmet_gold);
    _addItem(TOOLS, Item::chestplate_gold);
    _addItem(TOOLS, Item::leggings_gold);
    _addItem(TOOLS, Item::boots_gold);
    _addItem(TOOLS, Item::sword_gold);
    _addItem(TOOLS, Item::shovel_gold);
    _addItem(TOOLS, Item::pickAxe_gold);
    _addItem(TOOLS, Item::hatchet_gold);
    _addItem(TOOLS, Item::hoe_gold);

    _addItem(TOOLS, Item::fishingRod);
    _addItem(TOOLS, Item::map);

    _addItem(TOOLS, Item::helmet_diamond);
    _addItem(TOOLS, Item::chestplate_diamond);
    _addItem(TOOLS, Item::leggings_diamond);
    _addItem(TOOLS, Item::boots_diamond);
    _addItem(TOOLS, Item::sword_emerald);
    _addItem(TOOLS, Item::shovel_emerald);
    _addItem(TOOLS, Item::pickAxe_emerald);
    _addItem(TOOLS, Item::hatchet_emerald);
    _addItem(TOOLS, Item::hoe_emerald);

    _addTile(MISC, Tile::chest);
    _addTile(MISC, Tile::craftingTable);
    _addTile(MISC, Tile::furnace);
    //_addTile(MISC, Tile::jukeBox);
    _addTile(MISC, Tile::fence);
    //_addTile(MISC, Tile::ironBars);
    _addItem(MISC, Item::bed);
    _addItem(MISC, Item::bucket_empty);
    _addItem(MISC, Item::bucket_lava);
    _addItem(MISC, Item::bucket_water);
    _addItem(MISC, Item::milk);
    _addItem(MISC, Item::snowBall);
    _addItem(MISC, Item::paper);
    _addItem(MISC, Item::book);
    _addItem(MISC, Item::slimeBall);
    _addItem(MISC, Item::bone);
    //_addItem(MISC, Item::enderPearl);
    _addItem(MISC, Item::record_01);
    _addItem(MISC, Item::record_02);
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
    case TILE_TALL_GRASS:     maxAux = 3;  break;
    }
    for (int aux = 0; aux <= maxAux; aux++)
        _addTile(tab, tile, aux);
}

void CreativeMenu_Console::_addTile(Category tab, Tile* tile, TileData data)
{
    creativeItems[tab].push_back(ItemStack(tile, 1, data));
}

void CreativeMenu_Console::_addItem(Category tab, Item* item, int data)
{
    creativeItems[tab].push_back(ItemStack(item, 1, data));
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
