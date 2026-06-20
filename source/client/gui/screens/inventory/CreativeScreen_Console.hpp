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
    bool _nextTab();
    bool _prevTab();
    void _updateScroll(float) override;
    void _renderLabels() override;
    void _renderBg(float partialTicks) override;
    SlotDisplay _createSlotDisplay(const Slot& slot) override;

public:

    class CreativeTabLayout : public TabLayout
    {
    public:
        CreativeTabLayout(CreativeScreen_Console*);

        void onSelectElement(GuiElement*) override;
    };

    CreativeTabLayout m_tabLayout;
};
