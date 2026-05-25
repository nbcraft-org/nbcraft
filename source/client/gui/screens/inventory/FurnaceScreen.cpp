#include "FurnaceScreen.hpp"
#include "world/inventory/FurnaceMenu.hpp"
#include "renderer/ShaderConstants.hpp"

FurnaceScreen::FurnaceScreen(Inventory* inventory, FurnaceTileEntity* container)
    : ContainerScreen(new FurnaceMenu(inventory, container))
    , m_pInventory(inventory)
    , m_pFurnace(container)
{
}

void FurnaceScreen::init()
{
    if (m_uiTheme == UI_CONSOLE)
    {
        m_imageWidth = 432;
        m_imageHeight = 436;
    }
    ContainerScreen::init();
    if (m_uiTheme == UI_CONSOLE)
        m_topPos = Mth::Max(24, m_topPos - 48);
}

void FurnaceScreen::tick()
{
    ContainerScreen::tick();
}

void FurnaceScreen::_renderLabels()
{
    if (m_uiTheme == UI_CONSOLE)
    {
        m_pFont->drawScalable(m_pFurnace->getName(), 26, 21, 0x404040);
        m_pFont->drawScalable(m_pInventory->getName(), 26, m_imageHeight - 238 + 2, 0x404040);

        m_pFont->drawScalable("Ingredient", 40, 68, 0x404040);
        m_pFont->drawScalable("Fuel", 102, 162, 0x404040);
    }
    else
    {
        m_pFont->draw(m_pFurnace->getName(), 66, 6, 0x404040);
        m_pFont->draw(m_pInventory->getName(), 8, m_imageHeight - 96 + 2, 0x404040);
    }
}

void FurnaceScreen::_renderBg(float a)
{
    if (m_uiTheme == UI_CONSOLE)
    {
        currentShaderColor = Color::WHITE;
        blitNineSlice(*m_pMinecraft->m_pTextures, ScreenRenderer::PANEL_SLICES, m_leftPos, m_topPos, m_imageWidth, m_imageHeight, 32);
        blitSprite(*m_pMinecraft->m_pTextures, "gui/console/Graphics/Arrow_Off.png", m_leftPos + 224, m_topPos + 100, 72, 48);

        blitSprite(*m_pMinecraft->m_pTextures, "gui/console/Graphics/Flame_Off.png", m_leftPos + 147, m_topPos + 96, 48, 48);

        int p;
        if (m_pFurnace->isLit())
        {
            p = m_pFurnace->getLitProgress(12);
            blitSprite(*m_pMinecraft->m_pTextures, "gui/console/Graphics/Flame_On.png", m_leftPos + 195, m_topPos + 141, -48, -(p + 2) * 3, nullptr, 0.0f, 3.0f, 48, (p + 2) * 3);
        }

        p = m_pFurnace->getBurnProgress(24);
        blitSprite(*m_pMinecraft->m_pTextures, "gui/console/Graphics/Arrow_On.png", m_leftPos + 224, m_topPos + 100, (p + 1) * 3, 48, nullptr, 0.0f, 0.0f, (p + 1) * 3, 48);
        //blit(m_leftPos + 79, m_topPos + 34, 176, 14, p + 1, 16, 0, 0);
    }
    else
    {
        currentShaderColor = Color::WHITE;

        m_pMinecraft->m_pTextures->loadAndBindTexture("gui/furnace.png");

        blit(m_leftPos, m_topPos, 0, 0, m_imageWidth, m_imageHeight, 0, 0);

        int p;
        if (m_pFurnace->isLit())
        {
            p = m_pFurnace->getLitProgress(12);
            blit(m_leftPos + 56, m_topPos + 36 + 12 - p, 176, 12 - p, 14, p + 2, 0, 0);
        }

        p = m_pFurnace->getBurnProgress(24);
        blit(m_leftPos + 79, m_topPos + 34, 176, 14, p + 1, 16, 0, 0);
    }
}

SlotDisplay FurnaceScreen::_createSlotDisplay(const Slot& slot)
{
    if (m_uiTheme == UI_CONSOLE)
    {
        constexpr int slotSize = 42;
        switch (slot.m_group)
        {
        case Slot::INPUT:
            return SlotDisplay(154, 57 + (slot.m_stackId % 2) * ((slotSize + 5) * 2), slotSize, true);
        case Slot::OUTPUT:
            return SlotDisplay(312, 97, 64, true);
        case Slot::INVENTORY:
            return SlotDisplay(28 + (slot.m_stackId % 9) * slotSize, 230 + ((slot.m_stackId / 9) - 1) * slotSize, slotSize, true);
        case Slot::HOTBAR:
            return SlotDisplay(28 + (slot.m_stackId % 9) * slotSize, 369, slotSize, true);
        default:
            return SlotDisplay();
        }
    }
    else
    {
        constexpr int slotSize = 18;
        switch (slot.m_group)
        {
        case Slot::INPUT:
            return SlotDisplay(56, 17 + (slot.m_stackId % 2) * (slotSize * 2), slotSize);
        case Slot::OUTPUT:
            return SlotDisplay(116, 35);
        case Slot::INVENTORY:
            return SlotDisplay(8 + (slot.m_stackId % 9) * slotSize, 84 + ((slot.m_stackId / 9) - 1) * slotSize, slotSize);
        case Slot::HOTBAR:
            return SlotDisplay(8 + (slot.m_stackId % 9) * slotSize, 142, slotSize);
        default:
            return SlotDisplay();
        }
    }
}

void FurnaceScreen::renderBackground()
{
    if (m_uiTheme == UI_CONSOLE)
        return;

    ContainerScreen::renderBackground();
}
