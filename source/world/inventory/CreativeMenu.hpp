#pragma once

#include <memory>
#include <vector>
#include "ContainerMenu.hpp"
#include "Container.hpp"

class Player;

class CreativeMenu : public ContainerMenu
{
private:
    Container* m_container;

    static std::vector<ItemStack> s_creativeItems;
    static bool s_initialized;
    static void initCreativeItems();

public:
    CreativeMenu(Container* inventory, Container* container);

    static const std::vector<ItemStack>& getCreativeItems() { return s_creativeItems; }

    void updateScroll(float scroll);

    bool stillValid(Player* player) const override;
};
