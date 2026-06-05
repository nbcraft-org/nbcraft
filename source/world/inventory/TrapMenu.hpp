#pragma once

#include "ContainerMenu.hpp"
#include "world/entity/Player.hpp"
#include "world/tile/entity/DispenserTileEntity.hpp"

class TrapMenu : public ContainerMenu
{
public:
    TrapMenu(Inventory* inventory, DispenserTileEntity* container);

public:
    bool stillValid(Player* player) const override;

private:
    DispenserTileEntity* m_pTrap;
};
