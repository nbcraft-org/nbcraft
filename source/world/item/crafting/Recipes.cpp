#include "Recipes.hpp"
#include "world/tile/ClothTile.hpp"
//#include "world/tile/RecordPlayerTile.hpp"
//#include "world/item/MapItem.hpp"
#include "common/Logger.hpp"
#include "compat/GameVersion.h"

Recipes* Recipes::instance = nullptr;

void Recipes::addTools(const ItemStack& material, Item* sword, Item* pickaxe, Item* axe, Item* shovel, Item* hoe)
{
    add(ShapedRecipeBuilder("XXX",
                            " # ",
                            " # ", ItemStack(pickaxe))
        .add('X', material)
        .add('#', Item::stick), GC_TOOLS, 0);

    add(ShapedRecipeBuilder("X",
                            "#",
                            "#", ItemStack(shovel))
        .add('X', material)
        .add('#', Item::stick), GC_TOOLS, 1);

    add(ShapedRecipeBuilder("XX",
        "X#",
        " #", ItemStack(axe))
        .add('X', material)
        .add('#', Item::stick), GC_TOOLS, 2);

    add(ShapedRecipeBuilder("XX",
                            " #",
                            " #", ItemStack(hoe))
        .add('X', material)
        .add('#', Item::stick), GC_TOOLS, 3);

    add(ShapedRecipeBuilder("X",
        "X",
        "#", ItemStack(sword))
        .add('X', material)
        .add('#', Item::stick), GC_TOOLS, 9);
}

void Recipes::addArmor(const ItemStack& material, Item* helmet, Item* chestplate, Item* leggings, Item* boots, Group_Console group)
{
    add(ShapedRecipeBuilder("XXX",
                            "X X", ItemStack(helmet))
        .add('X', material), group, 0);

    add(ShapedRecipeBuilder("X X",
                            "XXX",
                            "XXX", ItemStack(chestplate))
        .add('X', material), group, 1);

    add(ShapedRecipeBuilder("XXX",
                            "X X",
                            "X X", ItemStack(leggings))
        .add('X', material), group, 2);

    add(ShapedRecipeBuilder("X X",
                            "X X", ItemStack(boots))
        .add('X', material), group, 3);
}

void Recipes::addOre(const ItemStack& material, Tile* block, int unprocessGroup)
{
    add(ShapedRecipeBuilder("XXX",
                            "XXX",
                            "XXX", ItemStack(block))
        .add('X', material), GC_DECORATION, 2);

    add(ShapedRecipeBuilder("X", ItemStack(material.getId(), 9, material.getAuxValue()))
        .add('X', block), GC_DECORATION, unprocessGroup);
}

class RecipeSorter
{
public:
    bool operator()(const Recipe* a, const Recipe* b)
    {
        if (!a->isShaped() && b->isShaped())
            return false;
        if (a->isShaped() && !b->isShaped())
            return true;
        return a->size() > b->size();
    }
};

Recipes::Recipes()
{
    // WeaponRecipes & ToolRecipes
    addTools(ItemStack(Tile::wood),       Item::sword_wood,    Item::pickAxe_wood,    Item::hatchet_wood,    Item::shovel_wood,    Item::hoe_wood);
    addTools(ItemStack(Tile::stoneBrick), Item::sword_stone,   Item::pickAxe_stone,   Item::hatchet_stone,   Item::shovel_stone,   Item::hoe_stone);
    addTools(ItemStack(Item::ironIngot),  Item::sword_iron,    Item::pickAxe_iron,    Item::hatchet_iron,    Item::shovel_iron,    Item::hoe_iron);
    addTools(ItemStack(Item::goldIngot),  Item::sword_stone,   Item::pickAxe_gold,    Item::hatchet_gold,    Item::shovel_gold,    Item::hoe_gold);
    addTools(ItemStack(Item::emerald),    Item::sword_emerald, Item::pickAxe_emerald, Item::hatchet_emerald, Item::shovel_emerald, Item::hoe_emerald);

#if MC_VERSION >= MC_VER_BETA(1, 7, 0)
    add(ShapedRecipeBuilder("A ",
                            " A", ItemStack(Item::shears, 1))
        .add('A', Item::ironIngot), GC_TOOLS, 4);
#endif

    add(ShapedRecipeBuilder("A ",
                            " B", ItemStack(Item::flintAndSteel, 1))
        .add('A', Item::ironIngot)
        .add('B', Item::flint), GC_TOOLS, 4);

    add(ShapedRecipeBuilder("X#X",
                            "#X#",
                            "X#X", ItemStack(Tile::tnt, 1))
        .add('X', Item::sulphur)
        .add('#', Tile::sand), GC_TOOLS, 5);

    add(ShapedRecipeBuilder("  #",
                            " #X",
                            "# X", ItemStack(Item::fishingRod, 1))
        .add('#', Item::stick)
        .add('X', Item::string), GC_TOOLS, 6);

    add(ShapedRecipeBuilder(" #X",
                            "# X",
                            " #X", ItemStack(Item::bow))
        .add('X', Item::string)
        .add('#', Item::stick), GC_TOOLS, 7);

    add(ShapedRecipeBuilder("X",
                            "#",
                            "Y", ItemStack(Item::arrow, 4))
        .add('Y', Item::feather)
        .add('X', Item::flint)
        .add('#', Item::stick), GC_TOOLS, 7);

    add(ShapedRecipeBuilder("# #",
                            " # ", ItemStack(Item::bucket_empty, 1))
        .add('#', Item::ironIngot), GC_TOOLS, 8);

    add(ShapedRecipeBuilder("# #",
                            " # ", ItemStack(Item::bowl, 4))
        .add('#', Tile::wood), GC_TOOLS, 8);

    add(ShapedRecipeBuilder("X",
                            "#", ItemStack(Tile::torch, 4))
        .add('X', ItemStack(Item::coal, 1, 1))
        .add('#', Item::stick), GC_TOOLS, 10);

    add(ShapedRecipeBuilder("X",
                            "#", ItemStack(Tile::torch, 4))
        .add('X', Item::coal)
        .add('#', Item::stick), GC_TOOLS, 10);

    add(ShapedRecipeBuilder("##",
                            "##", ItemStack(Tile::glowstone))
        .add('#', Item::yellowDust), GC_TOOLS, 10);

    add(ShapedRecipeBuilder("A",
        "B", ItemStack(Tile::pumpkinLantern, 1))
        .add('A', Tile::pumpkin)
        .add('B', Tile::torch), GC_TOOLS, 10);

    add(ShapedRecipeBuilder(" # ",
                            "#X#",
                            " # ", ItemStack(Item::clock, 1))
        .add('#', Item::goldIngot)
        .add('X', Item::redStone), GC_TOOLS, 11);

    add(ShapedRecipeBuilder(" # ",
                            "#X#",
                            " # ", ItemStack(Item::compass, 1))
        .add('#', Item::ironIngot)
        .add('X', Item::redStone), GC_TOOLS, 11);

    add(ShapedRecipeBuilder("###",
                            "#X#",
                            "###", ItemStack(Item::map, 1))
        .add('#', Item::paper)
        .add('X', Item::compass), GC_TOOLS, 11);

    // ArmorRecipes
    addArmor(ItemStack(Item::leather),   Item::helmet_cloth,   Item::chestplate_cloth,   Item::leggings_cloth,   Item::boots_cloth);
    addArmor(ItemStack(Tile::fire),      Item::helmet_chain,   Item::chestplate_chain,   Item::leggings_chain,   Item::boots_chain, GC_COUNT);
    addArmor(ItemStack(Item::ironIngot), Item::helmet_iron,    Item::chestplate_iron,    Item::leggings_iron,    Item::boots_iron);
    addArmor(ItemStack(Item::goldIngot), Item::helmet_gold,    Item::chestplate_gold,    Item::leggings_gold,    Item::boots_gold);
    addArmor(ItemStack(Item::emerald),   Item::helmet_diamond, Item::chestplate_diamond, Item::leggings_diamond, Item::boots_diamond);

    // StructureRecipes
    add(ShapedRecipeBuilder("#", ItemStack(Tile::wood, 4))
        .add('#', Tile::treeTrunk), GC_STRUCTURES, 0);

    add(ShapedRecipeBuilder("#",
                            "#", ItemStack(Item::stick, 4))
        .add('#', Tile::wood), GC_STRUCTURES, 1);

    add(ShapedRecipeBuilder("##",
                            "##", ItemStack(Tile::sandStone))
        .add('#', Tile::sand), GC_STRUCTURES, 2);

    add(ShapedRecipeBuilder("##",
                            "##", ItemStack(Tile::snow, 1))
        .add('#', Item::snowBall), GC_STRUCTURES, 2);

    add(ShapedRecipeBuilder("##",
                            "##", ItemStack(Tile::clay, 1))
        .add('#', Item::clay), GC_STRUCTURES, 2);

    add(ShapedRecipeBuilder("##",
                            "##", ItemStack(Tile::redBrick, 1))
        .add('#', Item::brick), GC_STRUCTURES, 2);

    add(ShapedRecipeBuilder("##",
                            "##", ItemStack(Tile::craftingTable))
        .add('#', Tile::wood), GC_STRUCTURES, 3);

    add(ShapedRecipeBuilder("###",
                            "# #",
                            "###", ItemStack(Tile::furnace))
        .add('#', Tile::stoneBrick), GC_STRUCTURES, 4);

    add(ShapedRecipeBuilder("###",
                            "# #",
                            "###", ItemStack(Tile::chest))
        .add('#', Tile::wood), GC_STRUCTURES, 5);

    add(ShapedRecipeBuilder("###",
                            "XXX", ItemStack(Item::bed))
        .add('#', Tile::cloth)
        .add('X', Tile::wood), GC_STRUCTURES, 6);

    add(ShapedRecipeBuilder("# #",
                            "###",
                            "# #", ItemStack(Tile::ladder, 2))
        .add('#', Item::stick), GC_STRUCTURES, 7);

    add(ShapedRecipeBuilder("###",
                            "###", ItemStack(Tile::fence, 2))
        .add('#', Item::stick), GC_STRUCTURES, 8);

#if MC_VERSION >= MC_VER_BETA(1, 8, 0)
    add(ShapedRecipeBuilder("#W#",
        "#W#", ItemStack(Tile::fenceGate, 1))
        .add('#', Item::stick)
        .add('W', Tile::wood), GC_STRUCTURES, 8);
#endif

    add(ShapedRecipeBuilder("##",
                            "##",
                            "##", ItemStack(Item::door_wood, 1))
        .add('#', Tile::wood), GC_STRUCTURES, 9);

    add(ShapedRecipeBuilder("##",
                            "##",
                            "##", ItemStack(Item::door_iron, 1))
        .add('#', Item::ironIngot), GC_STRUCTURES, 9);

    add(ShapedRecipeBuilder("###",
                            "###", ItemStack(Tile::trapDoor, 2))
        .add('#', Tile::wood), GC_STRUCTURES, 9);

    add(ShapedRecipeBuilder("#  ",
                            "## ",
                            "###", ItemStack(Tile::stairs_wood, 4))
        .add('#', Tile::wood), GC_STRUCTURES, 10);

    add(ShapedRecipeBuilder("#  ",
                            "## ",
                            "###", ItemStack(Tile::stairs_stone, 4))
        .add('#', Tile::stoneBrick), GC_STRUCTURES, 10);

    add(ShapedRecipeBuilder("###", ItemStack(Tile::stoneSlabHalf, 3, 2))
        .add('#', Tile::wood), GC_STRUCTURES, 11);

    add(ShapedRecipeBuilder("###", ItemStack(Tile::stoneSlabHalf, 3, 1))
        .add('#', Tile::sandStone), GC_STRUCTURES, 11);

    add(ShapedRecipeBuilder("###", ItemStack(Tile::stoneSlabHalf, 3, 3))
        .add('#', Tile::stoneBrick), GC_STRUCTURES, 11);

    add(ShapedRecipeBuilder("###", ItemStack(Tile::stoneSlabHalf, 3))
        .add('#', Tile::rock), GC_STRUCTURES, 11);

    // FoodRecipes
    add(ShapedRecipeBuilder("YX#", ItemStack(Item::mushroomStew, 1))
        .add('X', Tile::mushroom1)
        .add('Y', Tile::mushroom2)
        .add('#', Item::bowl), GC_FOOD);

    add(ShapedRecipeBuilder("#X#", ItemStack(Item::cookie, 1))
        .add('X', ItemStack(Item::dye_powder, 1, 3))
        .add('#', Item::wheat), GC_FOOD);

    add(ShapedRecipeBuilder("AAA",
                            "BEB",
                            "CCC", ItemStack(Item::cake, 1))
        .add('A', Item::milk)
        .add('B', Item::sugar)
        .add('C', Item::wheat)
        .add('E', Item::egg), GC_FOOD);

    add(ShapedRecipeBuilder("#", ItemStack(Item::sugar, 1))
        .add('#', Item::reeds), GC_FOOD);

    add(ShapedRecipeBuilder("###", ItemStack(Item::bread, 1))
        .add('#', Item::wheat), GC_FOOD);

    add(ShapedRecipeBuilder("###",
                            "#X#",
                            "###", ItemStack(Item::apple_gold, 1))
        .add('#', Tile::goldBlock)
        .add('X', Item::apple), GC_FOOD);

    add(ShapedRecipeBuilder("###",
                            "#X#",
                            "###", ItemStack(Tile::musicBlock, 1))
        .add('#', Tile::wood)
        .add('X', Item::redStone), GC_MECHANISM);

    //add(ShapedRecipeBuilder("X X",
    //                        "X#X",
    //                        "X X", ItemStack(Tile::rail, 16))
    //    .add('X', Item::ironIngot)
    //    .add('#', Item::stick));

    add(ShapedRecipeBuilder("# #",
                            "###", ItemStack(Item::minecart, 1))
        .add('#', Item::ironIngot), GC_TRANSPORT);

    add(ShapedRecipeBuilder("A",
                            "B", ItemStack(Item::minecart_chest, 1))
        .add('A', Tile::chest)
        .add('B', Item::minecart), GC_TRANSPORT);

    add(ShapedRecipeBuilder("A",
                            "B", ItemStack(Item::minecart_furnace, 1))
        .add('A', Tile::furnace)
        .add('B', Item::minecart), GC_TRANSPORT);

    add(ShapedRecipeBuilder("# #",
                            "###", ItemStack(Item::boat, 1))
        .add('#', Tile::wood), GC_TRANSPORT);


    //add(ShapedRecipeBuilder("X",
    //                        "#", ItemStack(Tile::lever, 1))
    //    .add('X', Item::stick)
    //    .add('#', Tile::stoneBrick));

    //add(ShapedRecipeBuilder("X",
    //                        "#", ItemStack(Tile::redstoneTorchLit, 1))
    //    .add('X', Item::redStone)
    //    .add('#', Item::stick));

    //add(ShapedRecipeBuilder("#X#",
    //                        "III", ItemStack(Item::diode, 1))
    //    .add('#', Tile::redstoneTorchLit)
    //    .add('X', Item::redStone)
    //    .add('I', Tile::rock));

    //add(ShapedRecipeBuilder("#", ItemStack(Tile::button, 1))
    //    .add('#', Tile::rock));

    //add(ShapedRecipeBuilder("##", ItemStack(Tile::stonePressurePlate, 1))
    //    .add('#', Tile::rock));

    //add(ShapedRecipeBuilder("##", ItemStack(Tile::woodPressurePlate, 1))
    //    .add('#', Tile::wood));

    //add(ShapedRecipeBuilder("###",
    //                        "#X#",
    //                        "#R#", ItemStack(Tile::dispenser, 1))
    //    .add('#', Tile::stoneBrick)
    //    .add('X', Item::bow)
    //    .add('R', Item::redStone));

#if MC_VERSION >= MC_VER_BETA(1, 7, 0)
    //add(ShapedRecipeBuilder("###",
    //                        "XIX",
    //                        "XRX", ItemStack(Tile::piston))
    //    .add('#', Tile::wood)
    //    .add('X', Tile::stoneBrick)
    //    .add('I', Item::ironIngot)
    //    .add('R', Item::redStone));

    //add(ShapedRecipeBuilder("X",
    //                        "#", ItemStack(Tile::stickyPiston))
    //    .add('X', Item::slimeBall)
    //    .add('#', Tile::piston));
#endif

    // ClothDyeRecipes
    for (int i = 0; i < 16; ++i)
    {
        add(ShapelessRecipeBuilder(ItemStack(Tile::cloth, 1, ClothTile::getColorFromData(i)))
            .add(ItemStack(Item::dye_powder, 1, i))
            .add(ItemStack(Tile::cloth, 1, 0)), GC_DECORATION, 0);
    }

    add(ShapedRecipeBuilder("###",
                            "###",
                            "###", ItemStack(Tile::cloth, 1))
        .add('#', Item::string), GC_DECORATION, 0);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 2, 11))
        .add(Tile::flower), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 2, 1))
        .add(Tile::rose), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 3, 15))
        .add(Item::bone), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 2, 9))
        .add(ItemStack(Item::dye_powder, 1, 1))
        .add(ItemStack(Item::dye_powder, 1, 15)), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 2, 14))
        .add(ItemStack(Item::dye_powder, 1, 1))
        .add(ItemStack(Item::dye_powder, 1, 11)), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 2, 10))
        .add(ItemStack(Item::dye_powder, 1, 2))
        .add(ItemStack(Item::dye_powder, 1, 15)), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 2, 8))
        .add(ItemStack(Item::dye_powder, 1, 0))
        .add(ItemStack(Item::dye_powder, 1, 15)), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 2, 7))
        .add(ItemStack(Item::dye_powder, 1, 8))
        .add(ItemStack(Item::dye_powder, 1, 15)), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 3, 7))
        .add(ItemStack(Item::dye_powder, 1, 0))
        .add(ItemStack(Item::dye_powder, 1, 15))
        .add(ItemStack(Item::dye_powder, 1, 15)), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 2, 12))
        .add(ItemStack(Item::dye_powder, 1, 4))
        .add(ItemStack(Item::dye_powder, 1, 15)), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 2, 6))
        .add(ItemStack(Item::dye_powder, 1, 4))
        .add(ItemStack(Item::dye_powder, 1, 2)), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 2, 5))
        .add(ItemStack(Item::dye_powder, 1, 4))
        .add(ItemStack(Item::dye_powder, 1, 1)), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 2, 13))
        .add(ItemStack(Item::dye_powder, 1, 5))
        .add(ItemStack(Item::dye_powder, 1, 9)), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 3, 13))
        .add(ItemStack(Item::dye_powder, 1, 4))
        .add(ItemStack(Item::dye_powder, 1, 1))
        .add(ItemStack(Item::dye_powder, 1, 9)), GC_DECORATION, 1);

    add(ShapelessRecipeBuilder(ItemStack(Item::dye_powder, 4, 13))
        .add(ItemStack(Item::dye_powder, 1, 4))
        .add(ItemStack(Item::dye_powder, 1, 1))
        .add(ItemStack(Item::dye_powder, 1, 1))
        .add(ItemStack(Item::dye_powder, 1, 15)), GC_DECORATION, 1);

    // OreRecipes
    addOre(ItemStack(Item::goldIngot), Tile::goldBlock);
    addOre(ItemStack(Item::ironIngot), Tile::ironBlock);
    addOre(ItemStack(Item::emerald), Tile::emeraldBlock, 4);
    addOre(ItemStack(Item::dye_powder, 1, 4), Tile::lapisBlock, 1);

    //add(ShapedRecipeBuilder("###",
    //                        "#X#",
    //                        "###", ItemStack(Tile::recordPlayer, 1))
    //    .add('#', Tile::wood)
    //    .add('X', Item::emerald));

    add(ShapedRecipeBuilder("###", ItemStack(Item::paper, 3))
        .add('#', Item::reeds));

    add(ShapedRecipeBuilder("#",
                            "#",
                            "#", ItemStack(Item::book, 1))
        .add('#', Item::paper));

    add(ShapedRecipeBuilder("###",
                            "XXX",
                            "###", ItemStack(Tile::bookshelf, 1))
        .add('#', Tile::wood)
        .add('X', Item::book));

    add(ShapedRecipeBuilder("###",
                            "#X#",
                            "###", ItemStack(Item::painting, 1))
        .add('#', Item::stick)
        .add('X', Tile::cloth));

    add(ShapedRecipeBuilder("###",
                            "###",
                            " X ", ItemStack(Item::sign, 1))
        .add('#', Tile::wood)
        .add('X', Item::stick));


    std::sort(m_recipes.begin(), m_recipes.end(), RecipeSorter());

    LOG_I("%d recipes loaded", m_recipes.size());
}

Recipes::~Recipes()
{
    for (size_t i = 0; i < m_recipes.size(); ++i)
    {
        delete m_recipes[i];
    }
}
