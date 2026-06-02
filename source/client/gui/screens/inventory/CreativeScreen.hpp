#pragma once

#include "ContainerScreen.hpp"

class CreativeScreen : public ContainerScreen
{
private:
    static Container* creativeGrid;

public:
    CreativeScreen(Container* inventory);

    void tick() override;
    void slotClicked(Slot* slot, Container::SlotID slotId, MouseButtonType button, bool quick) override;
    void pointerPressed(const MenuPointer& pointer, MouseButtonType button) override;
    void pointerReleased(const MenuPointer& pointer, MouseButtonType button) override;
    void handleScrollWheel(float force) override;

protected:
    void _renderLabels() override;
    void _renderBg(float partialTicks) override;
    SlotDisplay _createSlotDisplay(const Slot& slot) override;

private:
    float m_scrolled;
    bool m_bIsScrolling;
};
