#include "FurnaceMenu.h"

#include "FurnaceTileEntity.h"
#include "world/inventory/ResultSlot.hpp"
#include "world/inventory/Slot.hpp"


FurnaceMenu::FurnaceMenu(Inventory* inventory, FurnaceTileEntity* furnace)
    : ContainerMenu(Container::FURNACE)
    , m_pFurnace(furnace)
{
    // Slot 0: Input (Top slot of the furnace)
    // Slot 1: Fuel (Bottom slot of the furnace)
    // Slot 2: Output/Result (Right slot of the furnace)
    
    // Note: The actual coordinates (x, y) for drawing the slots will be handled by FurnaceScreen later.
    // For now, we just register them in order.
    
    addSlot(new Slot(m_pFurnace, 0, Slot::INPUT)); // Input
    addSlot(new Slot(m_pFurnace, 1, Slot::INPUT)); // Fuel
    addSlot(new ResultSlot(inventory->m_pPlayer, nullptr, m_pFurnace, 2)); // Result

    // Add the 27 slots for the player's main inventory
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 9; ++x) {
            addSlot(new Slot(inventory, x + y * 9 + 9, Slot::INVENTORY));
        }
    }

    // Add the 9 slots for the player's hotbar
    for (int i = 0; i < 9; ++i) {
        addSlot(new Slot(inventory, i, Slot::HOTBAR));
    }
}

bool FurnaceMenu::stillValid(Player* player) const
{
    // The FurnaceTileEntity will tell us if it's still valid (e.g., if the block was destroyed)
    return m_pFurnace->stillValid(player); 
}