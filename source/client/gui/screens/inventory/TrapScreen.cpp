#include "TrapScreen.hpp"
#include "world/inventory/TrapMenu.hpp"
#include "renderer/ShaderConstants.hpp"

TrapScreen::TrapScreen(Inventory* inventory, DispenserTileEntity* trap) :
    ContainerScreen(new TrapMenu(inventory, trap))
{
    m_uiTheme = UI_JAVA;
}

void TrapScreen::_renderLabels()
{
    m_pFont->draw("Dispenser", 60, 6, Color::TEXT_GREY);
    m_pFont->draw(m_pMinecraft->m_pLocalPlayer->m_pInventory->getName(), 8, m_imageHeight - 96 + 2, Color::TEXT_GREY);
}

void TrapScreen::_renderBg(float partialTick)
{
    currentShaderColor = Color::WHITE;
    m_pMinecraft->m_pTextures->loadAndBindTexture("gui/trap.png");

    blit(m_leftPos, m_topPos, 0, 0, m_imageWidth, m_imageHeight, 0, 0);
}

SlotDisplay TrapScreen::_createSlotDisplay(const Slot& slot)
{
    constexpr int slotSize = 18;
    switch (slot.m_group)
    {
    case Slot::CONTAINER:
        return SlotDisplay(61 + (slot.m_stackId % 3) * slotSize, 17 + (slot.m_stackId / 3) * slotSize);
    case Slot::INVENTORY:
        return SlotDisplay(8 + (slot.m_stackId % 9) * slotSize, 84 + ((slot.m_stackId / 9) - 1) * slotSize, slotSize);
    case Slot::HOTBAR:
        return SlotDisplay(8 + (slot.m_stackId % 9) * slotSize, 142, slotSize);
    default:
        return SlotDisplay();
    }
}
