#ifndef NBCRAFT_FURNACEMENU_H
#define NBCRAFT_FURNACEMENU_H

#include "world/inventory/ContainerMenu.hpp"
#include "world/item/Inventory.hpp"

// Forward declaration so the compiler knows this class exists
class FurnaceTileEntity;

class FurnaceMenu : public ContainerMenu {
public:
    FurnaceMenu(Inventory* inventory, FurnaceTileEntity* furnace);
    ~FurnaceMenu() override = default;

    bool stillValid(Player* player) const override;
    // slotsChanged is NOT needed for a furnace! The TileEntity handles smelting.

    // quickMoveStack is complex, we can leave it empty for now to prevent crashes
    ItemStack quickMoveStack(int index) override { return ItemStack::EMPTY; }

private:
    FurnaceTileEntity *m_pFurnace;
};

#endif //NBCRAFT_FURNACEMENU_H