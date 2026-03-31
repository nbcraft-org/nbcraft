#ifndef NBCRAFT_FURNACESCREEN_H
#define NBCRAFT_FURNACESCREEN_H

#include "ContainerScreen.hpp"
#include "world/inventory/FurnaceTileEntity.h"

class FurnaceScreen : public ContainerScreen {
private:
    FurnaceTileEntity* m_pFurnace;

public:
    FurnaceScreen(Inventory* inventory, FurnaceTileEntity* furnace);

    // Fixed the C++11 warning here!
    ~FurnaceScreen() override {}

protected:
    // These are the exact 3 pure virtual methods NBCraft demands!
    void _renderLabels() override;
    void _renderBg(float partialTick) override;
    SlotDisplay _createSlotDisplay(const Slot& slot) override;
};

#endif // NBCRAFT_FURNACESCREEN_H