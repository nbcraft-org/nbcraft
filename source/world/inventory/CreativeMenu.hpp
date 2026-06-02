#pragma once

#include <memory>
#include "ContainerMenu.hpp"
#include "Container.hpp"

class Player;

class CreativeMenu : public ContainerMenu
{
private:
    Container* m_container;

public:
    CreativeMenu(Container* inventory, Container* container);

    void updateScroll(float scroll);

    bool stillValid(Player* player) const override;

public:
    std::vector<ItemStack> m_creativeItems;
};
