#pragma once

#include "ContainerMenu.hpp"
#include "CraftingContainer.hpp"
#include "ResultContainer.hpp"

class InventoryMenu : public ContainerMenu
{
public:
    InventoryMenu(Inventory* inventory, bool active = true);
    ~InventoryMenu();

    void slotsChanged(Container* container) override;
    void removed(Player* player) override;
    bool stillValid(Player* player) const override;
    ItemStack quickMoveStack(Container::SlotID slotId) override;

public:
    CraftingContainer* m_pCraftSlots;
    ResultContainer* m_pResultSlots;
    // Unused member
    bool m_bActive;
};
