#include "CreativeScreen.hpp"
#include "world/inventory/CreativeMenu.hpp"
#include "world/inventory/SimpleContainer.hpp"
#include "world/item/Inventory.hpp"
#include "world/entity/Player.hpp"
#include "client/gui/screens/inventory/InventoryScreen.hpp"
#include "renderer/ShaderConstants.hpp"

Container* CreativeScreen::creativeGrid = new SimpleContainer(72, "Item selection");

CreativeScreen::CreativeScreen(Container* inventory) : ContainerScreen(new CreativeMenu(inventory, creativeGrid))
{
    m_imageHeight = 208;
    m_scrolled = 0.0f;
    m_bIsScrolling = false;
}

void CreativeScreen::slotClicked(Slot* slot, Container::SlotID slotId, MouseButtonType button, bool quick)
{
    Inventory* inv = m_pMinecraft->m_pLocalPlayer->m_pInventory;
    ItemStack carried = inv->getCarried();

    if (slot)
    {
        if (slot->m_pContainer == creativeGrid)
        {
            ItemStack& slotItem = slot->getItem();

            if (!carried.isEmpty() && slotItem.isValid() && carried.getId() == slotItem.getId())
            {
                if (button == MOUSE_BUTTON_LEFT)
                {
                    if (quick)
                        carried.m_count = carried.getMaxStackSize();
                    else if (carried.m_count < carried.getMaxStackSize())
                        carried.m_count++;
                }
                else if (button == MOUSE_BUTTON_RIGHT)
                {
                    if (carried.m_count <= 1)
                    {
                        inv->setCarried(ItemStack());
                        return;
                    }
                    else
                    {
                        carried.m_count--;
                    }
                }
                inv->setCarried(carried);
            }
            else if (!carried.isEmpty())
            {
                inv->setCarried(ItemStack());
            }
            else if (!slotItem.isValid())
            {
                inv->setCarried(ItemStack());
            }
            else if (carried.isEmpty() || carried.getId() != slotItem.getId())
            {
                ItemStack* newCarried = slotItem.copy();
                if (newCarried)
                {
                    if (quick)
                        newCarried->m_count = newCarried->getMaxStackSize();
                    inv->setCarried(*newCarried);
                    delete newCarried;
                }
            }
        }
        else
        {
            ContainerScreen::slotClicked(slot, slotId, button, quick);
        }
    }
    else
    {
        if (!carried.isEmpty())
        {
            if (button == MOUSE_BUTTON_LEFT)
            {
                m_pMinecraft->m_pLocalPlayer->drop(carried);
                inv->setCarried(ItemStack());
            }
            else if (button == MOUSE_BUTTON_RIGHT)
            {
                ItemStack dropped = carried.remove(1);
                m_pMinecraft->m_pLocalPlayer->drop(dropped);
                if (carried.m_count <= 0)
                    inv->setCarried(ItemStack());
            }
        }
    }
}

void CreativeScreen::_renderLabels()
{
    m_pFont->draw(creativeGrid->getName(), 8, 6, 0x404040);
}

void CreativeScreen::_renderBg(float partialTicks)
{
    currentShaderColor = Color::WHITE;

    m_pMinecraft->m_pTextures->loadAndBindTexture("gui/allitems.png");

    blit(m_leftPos, m_topPos, 0, 0, m_imageWidth, m_imageHeight, m_imageWidth, m_imageHeight);
    blit(m_leftPos + 154, m_topPos + 17 + (int)(145 * m_scrolled), 0, 208, 16, 16, 16, 16);
}

SlotDisplay CreativeScreen::_createSlotDisplay(const Slot& slot)
{
    if (slot.m_group == Slot::HOTBAR)
    {
        return SlotDisplay(8 + slot.m_stackId * 18, 184);
    }

    int col = slot.m_stackId % 8;
    int row = slot.m_stackId / 8;
    return SlotDisplay(8 + col * 18, 18 + row * 18);
}

void CreativeScreen::pointerPressed(const MenuPointer& pointer, MouseButtonType button)
{
    ContainerScreen::pointerPressed(pointer, button);

    if (pointer.x >= m_leftPos + 155 && pointer.x < m_leftPos + 169 && pointer.y >= m_topPos + 17 && pointer.y < m_topPos + 179)
        m_bIsScrolling = true;
}

void CreativeScreen::pointerReleased(const MenuPointer& pointer, MouseButtonType button)
{
    ContainerScreen::pointerReleased(pointer, button);
    m_bIsScrolling = false;
}

void CreativeScreen::handleScrollWheel(float force)
{
    CreativeMenu* creativeMenu = (CreativeMenu*)m_pMenu;
    creativeMenu->updateScroll(m_scrolled = Mth::clamp(m_scrolled - (force / ((float)CreativeMenu::GetCreativeItems().size() / 8 - 8 + 1)), 0.0f, 1.0f));
}

void CreativeScreen::tick()
{
    if (m_pMinecraft->m_pLocalPlayer->isSurvival())
    {
        m_pMinecraft->setScreen(new InventoryScreen(m_pMinecraft->m_pLocalPlayer));
        return;
    }

    if (m_bIsScrolling)
    {
        CreativeMenu* creativeMenu = (CreativeMenu*)m_pMenu;
        creativeMenu->updateScroll(m_scrolled = Mth::clamp((float(m_menuPointer.y) - (m_topPos + 25)) / 144.0f, 0.0f, 1.0f));
    }
}
