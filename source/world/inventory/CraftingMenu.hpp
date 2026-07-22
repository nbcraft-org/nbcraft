#pragma once

#include "ContainerMenu.hpp"
#include "CraftingContainer.hpp"
#include "ResultContainer.hpp"

class CraftingMenu : public ContainerMenu
{
public:
    CraftingMenu(Inventory* inventory, const TilePos& tilePos, bool is2x2 = false);
    ~CraftingMenu();

    void slotsChanged(Container* container) override;
    void removed(Player& player) override;
    bool stillValid(Player& player) const override;
    ItemStack quickMoveStack(Container::SlotID slotId) override;

private:
    const TilePos m_pos;

public:
    CraftingContainer* m_pCraftSlots;
    ResultContainer* m_pResultSlots;
};
