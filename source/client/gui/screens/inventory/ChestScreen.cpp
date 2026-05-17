#include "ChestScreen.hpp"
#include "world/inventory/ChestMenu.hpp"
#include "renderer/ShaderConstants.hpp"

ChestScreen::ChestScreen(Container* inventory, Container* container) : ContainerScreen(new ChestMenu(inventory, container)),
      m_pInventory(inventory),
      m_pContainer(container)
{
    m_containerRows = m_pContainer->getContainerSize() / 9;
}

void ChestScreen::init()
{
    if (m_uiTheme == UI_CONSOLE)
    {
        constexpr int defaultHeight = 412;
        constexpr int noRowHeight = defaultHeight - 126;
        m_imageWidth = 432;
        m_imageHeight = noRowHeight + m_containerRows * 42;
    }
    else
    {
        constexpr int defaultHeight = 222;
        constexpr int noRowHeight = defaultHeight - 108;
        m_imageHeight = noRowHeight + m_containerRows * 18;
    }

    ContainerScreen::init();

    if (m_uiTheme == UI_CONSOLE)
        m_topPos = Mth::Max(24, m_topPos - 48);
}

void ChestScreen::_renderLabels()
{
    if (m_uiTheme == UI_CONSOLE)
    {
        m_pFont->drawScalable(m_pContainer->getName(), 28, 20, Color::TEXT_GREY);
        m_pFont->drawScalable(m_pInventory->getName(), 28, m_imageHeight - 234 + 8, Color::TEXT_GREY);
    }
    else
    {
        m_pFont->draw(m_pContainer->getName(), 8, 6, Color::TEXT_GREY);
        m_pFont->draw(m_pInventory->getName(), 8, m_imageHeight - 96 + 2, Color::TEXT_GREY);
    }
}

void ChestScreen::_renderBg(float partialTicks)
{
    if (m_uiTheme == UI_CONSOLE)
    {
        blitNineSlice(*m_pMinecraft->m_pTextures, ScreenRenderer::PANEL_SLICES, m_leftPos, m_topPos, m_imageWidth, m_imageHeight, 32);
    }
    else
    {
        m_pMinecraft->m_pTextures->loadAndBindTexture("gui/container.png");
        currentShaderColor = Color::WHITE;

        blit(m_leftPos, m_topPos, 0, 0, m_imageWidth, m_containerRows * 18 + 17, 0, 0);
        blit(m_leftPos, m_topPos + m_containerRows * 18 + 17, 0, 126, m_imageWidth, 96, 0, 0);
    }
}

SlotDisplay ChestScreen::_createSlotDisplay(const Slot& slot)
{
    if (m_uiTheme == UI_CONSOLE)
    {
        constexpr int slotSize = 42;
        int rows = m_pContainer->getContainerSize() / 9;
        switch (slot.m_group)
        {
        case Slot::CONTAINER:
            return SlotDisplay(28 + (slot.m_stackId % 9) * slotSize, 50 + (slot.m_stackId / 9) * slotSize, slotSize, true);
        case Slot::INVENTORY:
            return SlotDisplay(28 + (slot.m_stackId % 9) * slotSize, (rows * 42) + 45 + (slot.m_stackId / 9) * slotSize, slotSize, true);
        case Slot::HOTBAR:
            return SlotDisplay(28 + (slot.m_stackId % 9) * slotSize, 226 + rows * slotSize, slotSize, true);
        default:
            return SlotDisplay();
        }
    }
    else
    {
        constexpr int slotSize = 18;
        int rows = m_pContainer->getContainerSize() / 9;
        switch (slot.m_group)
        {
        case Slot::CONTAINER:
            return SlotDisplay(8 + (slot.m_stackId % 9) * slotSize, 18 + (slot.m_stackId / 9) * slotSize, slotSize);
        case Slot::INVENTORY:
            return SlotDisplay(8 + (slot.m_stackId % 9) * slotSize, (rows * 18) + 13 + (slot.m_stackId / 9) * slotSize, slotSize);
        case Slot::HOTBAR:
            return SlotDisplay(8 + (slot.m_stackId % 9) * slotSize, 89 + rows * slotSize, slotSize);
        default:
            return SlotDisplay();
        }
    }
}

void ChestScreen::renderBackground()
{
    if (m_uiTheme == UI_CONSOLE)
        return;

    ContainerScreen::renderBackground();
}
