#pragma once

#include <memory>
#include <vector>
#include "ContainerMenu.hpp"
#include "Container.hpp"

class Player;

class CreativeMenu_Console : public ContainerMenu
{
public:
    enum Group
    {
        STRUCTURES,
        DECORATION,
        REDSTONE_AND_TRANSPORT,
        MATERIALS,
        FOOD,
        TOOLS,
        MISC,
        GROUP_COUNT
    };

    CreativeMenu_Console(Container* inventory, Container* container);

    void updateGrid(float scroll, int tab = STRUCTURES);

    bool stillValid(Player* player) const override;

    static const std::vector<ItemStack>& GetCreativeItems(int tab) { return creativeItems[tab]; }

private:
    static void initCreativeItems();
    static void _addAuxTile(Group, Tile* tile);
    static void _addTile(Group, Tile* tile, TileData = 0);
    static void _addItem(Group, Item* item, int data = 0);

private:
    Container* m_container;

    static std::vector<ItemStack> creativeItems[GROUP_COUNT];
    static bool initialized;
};
