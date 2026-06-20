#include "CreativeScreen_Console.hpp"
#include "world/inventory/SimpleContainer.hpp"
#include "world/item/Inventory.hpp"
#include "world/entity/Player.hpp"
#include "world/inventory/CreativeMenu_Console.hpp"
#include "client/gui/components/TabButton.hpp"
#include "client/locale/Language.hpp"
#include "renderer/ShaderConstants.hpp"

Container* CreativeScreen_Console::creativeGrid = new SimpleContainer(55, "Item selection");

CreativeScreen_Console::CreativeScreen_Console(Container* inventory) : CreativeScreen(new CreativeMenu_Console(inventory, creativeGrid))
, m_tabLayout(this)
{
    m_uiTheme = UI_CONSOLE;
    m_imageWidth = 647;
    m_imageHeight = 490;

    _addTab("gui/console/icon_structures.png", "container.tab.structures");
    _addTab("gui/console/icon_decoration.png", "container.tab.decoration");
    _addTab("gui/console/icon_Redstone_and_Transport.png", "container.tab.redstone_and_transport");
    _addTab("gui/console/icon_materials.png", "container.tab.materials");
    _addTab("gui/console/icon_food.png", "container.tab.food");
    _addTab("gui/console/icon_tools.png", "container.tab.tools");
    _addTab("gui/console/icon_misc.png", "container.tab.misc");
}

void CreativeScreen_Console::render(float partialTicks)
{
    blitTexture(*m_pMinecraft->m_pTextures, "gui/console/Graphics/Creative_Panel_7.png", m_leftPos, m_topPos, 0, 0, m_imageWidth, m_imageHeight, m_imageWidth, m_imageHeight);
    CreativeScreen::render(partialTicks);

    currentShaderColor = Color::WHITE;
    m_tabLayout.renderSelected(m_pMinecraft, m_menuPointer);
}

void CreativeScreen_Console::renderBackground()
{
}

void CreativeScreen_Console::init()
{
    CreativeScreen::init();
    m_tabLayout.init(m_leftPos, m_topPos - 3, -3);
}

bool CreativeScreen_Console::_isCreativeSlot(Slot* slot)
{
    return slot->m_pContainer == creativeGrid;
}

void CreativeScreen_Console::_addTab(const std::string& sprite, const std::string& name)
{
    m_tabLayout.m_elements.push_back(new TabButton(95, 78, name, SpriteDef(m_tabLayout.m_elements.empty() ? "gui/console/Graphics/Tab_Creative7_L.png" : m_tabLayout.m_elements.size() == 6 ? "gui/console/Graphics/Tab_Creative7_R.png" : "gui/console/Graphics/Tab_Creative7_M.png", IntRectangle()), SpriteDef(), SpriteDef(sprite, IntRectangle(0, 5, 48, 48))));
}

bool CreativeScreen_Console::_nextTab()
{
    m_tabLayout.areaNavigation(AreaNavigation::RIGHT);
    return true;
}

bool CreativeScreen_Console::_prevTab()
{
    m_tabLayout.areaNavigation(AreaNavigation::LEFT);
    return true;
}

void CreativeScreen_Console::_updateScroll(float)
{
}

void CreativeScreen_Console::_renderLabels()
{
    if (m_tabLayout.m_pSelectedElement)
    {
        const std::string& message = Language::get(m_tabLayout.m_pSelectedElement->getMessage());
        m_pFont->drawScalable(message, m_imageWidth / 2 - m_pFont->width(message), 91, Color::TEXT_GREY);
    }
}

void CreativeScreen_Console::_renderBg(float partialTicks)
{
}

SlotDisplay CreativeScreen_Console::_createSlotDisplay(const Slot& slot)
{
    const int slotSize = 54;
    if (slot.m_group == Slot::HOTBAR)
    {
        return SlotDisplay(83 + slot.m_stackId * slotSize, 414, slotSize, true);
    }

    int col = slot.m_stackId % 11;
    int row = slot.m_stackId / 11;
    return SlotDisplay(29 + col * slotSize, 124 + row * slotSize, slotSize, true);
}

CreativeScreen_Console::CreativeTabLayout::CreativeTabLayout(CreativeScreen_Console* screen) : TabLayout(screen)
{
}

void CreativeScreen_Console::CreativeTabLayout::onSelectElement(GuiElement* element)
{
    ((CreativeMenu_Console*)((CreativeScreen_Console*)m_pScreen)->m_pMenu)->updateGrid(0.0f, getIndex());
}
