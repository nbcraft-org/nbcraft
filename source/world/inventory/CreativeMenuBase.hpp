#pragma once

#include <vector>
#include "ContainerMenu.hpp"

class Player;

class CreativeMenuBase : public ContainerMenu
{
public:
    CreativeMenuBase();

    bool stillValid(Player& player) const override;

    static const std::vector<ItemStack>& GetCreativeItems() { return creativeItems; }

protected:
    static void initCreativeItems();

protected:
    static std::vector<ItemStack> creativeItems;
    static bool initialized;
};
