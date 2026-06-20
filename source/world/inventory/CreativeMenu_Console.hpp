#pragma once

#include <memory>
#include <vector>
#include "ContainerMenu.hpp"
#include "Container.hpp"

class Player;

class CreativeMenu_Console : public ContainerMenu
{
public:
    enum Category
    {
        STRUCTURES,
        DECORATION,
        REDSTONE_AND_TRANSPORT,
        MATERIALS,
        FOOD,
        TOOLS,
        MISC
    };

    CreativeMenu_Console(Container* inventory, Container* container);

    void updateGrid(float scroll, int tab = STRUCTURES);

    bool stillValid(Player* player) const override;

    static const std::vector<ItemStack>& GetCreativeItems(int tab) { return creativeItems[tab]; }

private:
    static void initCreativeItems();
    static void _addAuxTile(Category, Tile* tile);
    static void _addTile(Category, Tile* tile, TileData = 0);
    static void _addItem(Category, Item* item, int data = 0);

private:
    Container* m_container;

    static std::vector<ItemStack> creativeItems[8];
    static bool initialized;
};
