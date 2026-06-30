#pragma once

#include "ContainerScreen.hpp"
#include "world/tile/entity/FurnaceTileEntity.hpp"

class FurnaceScreen : public ContainerScreen
{
public:
    FurnaceScreen(Inventory* inventory, FurnaceTileEntity* container);
    void init() override;

public:
    void tick() override;

protected:
    void _renderLabels() override;
    void _renderBg(float a) override;
    SlotDisplay _createSlotDisplay(const Slot&) override;

public:
    void renderBackground() override;

private:
    Inventory* m_pInventory;
    FurnaceTileEntity* m_pFurnace;
};
