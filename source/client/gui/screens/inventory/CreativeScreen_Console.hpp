#pragma once

#include "ContainerScreen.hpp"
#include "client/gui/TabLayout.hpp"

class CreativeScreen_Console : public ContainerScreen
{
private:
    static Container* creativeGrid;

public:
    class CreativeTabLayout : public TabLayout
    {
    public:
        CreativeTabLayout(CreativeScreen_Console*);

        void onSelectElement(GuiElement*) override;

    private:
        CreativeScreen_Console* getScreen()
        {
            return (CreativeScreen_Console*) m_pScreen;
        }
    };

    CreativeScreen_Console(Container* inventory);

    void renderBackground() override;
    void slotClicked(Slot* slot, Container::SlotID slotId, MouseButtonType button, bool quick) override;

protected:
    void init() override;
    void _addTab(const std::string& name, const std::string& sprite);
    bool _nextTab();
    bool _prevTab();
    void _renderLabels() override;
    void _renderFg(float partialTicks) override;
    void _renderBg(float partialTicks) override;
    SlotDisplay _createSlotDisplay(const Slot& slot) override;

public:
    CreativeTabLayout m_tabLayout;
};
