#pragma once

#include "ContainerScreen.hpp"

class TrapScreen_Console : public ContainerScreen
{
public:
    TrapScreen_Console(Inventory* inventory, DispenserTileEntity* trap);

    void renderBackground() override;

protected:
    void _renderLabels() override;
    void _renderBg(float partialTick) override;
    SlotDisplay _createSlotDisplay(const Slot&) override;
};
