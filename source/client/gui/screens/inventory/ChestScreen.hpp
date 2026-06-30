#pragma once

#include "ContainerScreen.hpp"

class ChestScreen : public ContainerScreen
{
public:
    ChestScreen(Container* inventory, Container* container);
    void init() override;

protected:
    void _renderLabels() override;
    void _renderBg(float partialTicks) override;
    SlotDisplay _createSlotDisplay(const Slot&);

public:
    void renderBackground() override;

private:
    Container* m_pInventory;
    Container* m_pContainer;
    int m_containerRows;
};
