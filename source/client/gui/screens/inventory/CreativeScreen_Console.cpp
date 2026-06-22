#include "CreativeScreen_Console.hpp"
#include "world/inventory/SimpleContainer.hpp"
#include "world/item/Inventory.hpp"
#include "world/entity/Player.hpp"
#include "world/inventory/CreativeMenu_Console.hpp"
#include "client/gui/components/TabButton.hpp"
#include "client/locale/Language.hpp"
#include "renderer/ShaderConstants.hpp"

Container* CreativeScreen_Console::creativeGrid = new SimpleContainer(55, "Item selection");

CreativeScreen_Console::CreativeScreen_Console(Container* inventory) : ContainerScreen(new CreativeMenu_Console(inventory, creativeGrid))
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

void CreativeScreen_Console::_renderFg(float partialTicks)
{
    ContainerScreen::_renderFg(partialTicks);
    m_tabLayout.renderSelected(m_pMinecraft, m_menuPointer);
}

void CreativeScreen_Console::renderBackground()
{
}

void CreativeScreen_Console::slotClicked(Slot* slot, Container::SlotID slotId, MouseButtonType button, bool quick)
{
    Inventory* inv = m_pMinecraft->m_pLocalPlayer->m_pInventory;
    ItemStack& carried = inv->getCarried();

    if (slot)
    {
        if (slot->m_pContainer == creativeGrid)
        {
            ItemStack& slotItem = slot->getItem();

            if (!carried.isEmpty() && !slotItem.isEmpty() && carried.getId() == slotItem.getId())
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
                        inv->setCarried(ItemStack::EMPTY);
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
                inv->setCarried(ItemStack::EMPTY);
            }
            else if (slotItem.isEmpty())
            {
                inv->setCarried(ItemStack::EMPTY);
            }
            else if (carried.isEmpty() || carried.getId() != slotItem.getId())
            {
                ItemStack picked = slotItem;
                if (quick)
                    picked.m_count = picked.getMaxStackSize();

                ItemStack toMove = picked;

                m_pMenu->moveItemStackTo(toMove, 55, 64, false);

                if (!toMove.isEmpty())
                {
                    inv->setCarried(picked);
                    _selectSlot(m_pMenu->getSlot(63));
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
                inv->setCarried(ItemStack::EMPTY);
            }
            else if (button == MOUSE_BUTTON_RIGHT)
            {
                ItemStack dropped = carried.remove(1);
                m_pMinecraft->m_pLocalPlayer->drop(dropped);
                if (carried.m_count <= 0)
                    inv->setCarried(ItemStack::EMPTY);
            }
        }
    }
}

void CreativeScreen_Console::init()
{
    ContainerScreen::init();
    m_tabLayout.init(m_leftPos, m_topPos - 3, -3);
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
    currentShaderColor = Color::WHITE;
    blitTexture(*m_pMinecraft->m_pTextures, "gui/console/Graphics/Creative_Panel_7.png", m_leftPos, m_topPos, 0, 0, m_imageWidth, m_imageHeight, m_imageWidth, m_imageHeight);
}

SlotDisplay CreativeScreen_Console::_createSlotDisplay(const Slot& slot)
{
    constexpr int slotSize = 54;
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
    ((CreativeMenu_Console*)getScreen()->m_pMenu)->updateGrid(0.0f, getIndex());
}
