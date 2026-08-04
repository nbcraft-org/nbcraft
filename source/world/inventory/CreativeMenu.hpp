#pragma once

#include <memory>
#include <vector>
#include "ContainerMenu.hpp"
#include "Container.hpp"

class Player;

class CreativeMenu : public ContainerMenu
{
public:
    CreativeMenu(Container* inventory, Container* container);

    void updateScroll(float scroll);

    bool stillValid(Player* player) const override;

    static const std::vector<ItemStack>& GetCreativeItems() { return creativeItems; }

private:
    static void initCreativeItems();

    Container* m_container;

    static std::vector<ItemStack> creativeItems;
    static bool initialized;
};
