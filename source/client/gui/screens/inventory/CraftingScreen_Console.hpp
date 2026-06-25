#pragma once

#include "ContainerScreen.hpp"
#include "client/gui/TabLayout.hpp"
#include "client/gui/components/CraftButton_Console.hpp"

class CraftingScreen_Console : public ContainerScreen
{
private:
    static Container* creativeGrid;

public:
    class CraftingTabLayout : public TabLayout
    {
    public:
        CraftingTabLayout(CraftingScreen_Console*);

        void onSelectElement(GuiElement*) override;

    private:
        CraftingScreen_Console* getScreen()
        {
            return (CraftingScreen_Console*) m_pScreen;
        }
    };

    CraftingScreen_Console(Inventory* inventory, const TilePos& tilePos, Level* level, bool is2x2);

    void renderBackground() override;
    void onClose() override;
    void handleUserAction(const ActionInfo& action) override;

protected:
    void init() override;
    void _addTab(const std::string& name, const std::string& sprite);
    bool _areaNavigation(AreaNavigation::Direction) override;
    bool _nextTab() override;
    bool _prevTab() override;
    void _renderLabels() override;
    void _renderFg(float partialTicks) override;
    void _renderBg(float partialTicks) override;
    SlotDisplay _createSlotDisplay(const Slot& slot) override;

public:
    CraftingTabLayout m_tabLayout;
    CraftingTabLayout m_craftLayout;
    ClientCraftContext m_craftContext;
};
