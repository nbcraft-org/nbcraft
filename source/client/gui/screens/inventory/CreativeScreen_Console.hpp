#pragma once

#include "CreativeScreen.hpp"
#include "client/gui/TabLayout.hpp"

class CreativeScreen_Console : public CreativeScreen
{
private:
    static Container* creativeGrid;

public:
    CreativeScreen_Console(Container* inventory);

    void render(float partialTicks) override;
    void renderBackground() override;

protected:
    void init() override;
    bool _isCreativeSlot(Slot*) override;
    void _addTab(const std::string& name, const std::string& sprite);
    void _guiElementClicked(GuiElement& element) override;
    void _updateScroll(float) override;
    void _renderLabels() override;
    void _renderBg(float partialTicks) override;
    SlotDisplay _createSlotDisplay(const Slot& slot) override;

public:
    TabLayout m_tabLayout;
};
