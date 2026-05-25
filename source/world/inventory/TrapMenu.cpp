#include "TrapMenu.hpp"
#include "Slot.hpp"

TrapMenu::TrapMenu(Inventory* inventory, DispenserTileEntity* trap)
    : ContainerMenu(Container::DISPENSER), m_trap(trap)
{
    for (int y = 0; y < 3; ++y)
    {
        for (int x = 0; x < 3; ++x)
            addSlot(new Slot(m_trap, x + y * 3, Slot::CONTAINER));
    }

    for (int y = 0; y < 3; ++y)
    {
        for (int x = 0; x < 9; ++x)
            addSlot(new Slot(inventory, x + (y + 1) * 9, Slot::INVENTORY));
    }

    for (int i = 0; i < 9; ++i)
    {
        addSlot(new Slot(inventory, i, Slot::HOTBAR));
    }
}

bool TrapMenu::stillValid(Player* player) const
{
    return m_trap->stillValid(player);
}
