#pragma once

#include "ContainerScreen.hpp"
#include "world/tile/entity/DispenserTileEntity.hpp"

class TrapScreen : public ContainerScreen
{

public:
    TrapScreen(Inventory* inventory, DispenserTileEntity* container);

protected:
    void _renderLabels() override;
    void _renderBg(float partialTick) override;
    SlotDisplay _createSlotDisplay(const Slot&);
};
