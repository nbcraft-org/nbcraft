#include "TrapScreen_Console.hpp"
#include "world/inventory/TrapMenu.hpp"
#include "renderer/ShaderConstants.hpp"

const std::string CONTAINER_NAME = "Dispenser";

TrapScreen_Console::TrapScreen_Console(Inventory* inventory, DispenserTileEntity* trap)
    : ContainerScreen(new TrapMenu(inventory, trap))
{
    m_uiTheme = UI_CONSOLE;
    m_imageWidth = 428;
    m_imageHeight = 450;
}

void TrapScreen_Console::renderBackground()
{
}

void TrapScreen_Console::_renderLabels()
{
    m_pFont->drawScalable(CONTAINER_NAME, m_imageWidth / 2 - m_pFont->width(CONTAINER_NAME), 17, Color::TEXT_GREY);
    m_pFont->drawScalable(m_pMinecraft->m_pLocalPlayer->m_pInventory->getName(), 28, 213, Color::TEXT_GREY);
}

void TrapScreen_Console::_renderBg(float partialTick)
{
    currentShaderColor = Color::WHITE;
    blitNineSlice(*m_pMinecraft->m_pTextures, ScreenRenderer::PANEL_SLICES, m_leftPos, m_topPos, m_imageWidth, m_imageHeight, 32);
}

SlotDisplay TrapScreen_Console::_createSlotDisplay(const Slot& slot)
{
    constexpr int slotSize = 42;
    switch (slot.m_group)
    {
    case Slot::CONTAINER:
        return SlotDisplay(154 + (slot.m_stackId % 3) * slotSize, 64 + (slot.m_stackId / 3) * slotSize, slotSize, true);
    case Slot::INVENTORY:
        return SlotDisplay(28 + (slot.m_stackId % 9) * slotSize, 240 + ((slot.m_stackId / 9) - 1) * slotSize, slotSize, true);
    case Slot::HOTBAR:
        return SlotDisplay(28 + (slot.m_stackId % 9) * slotSize, 379, slotSize, true);
    default:
        return SlotDisplay();
    }
}