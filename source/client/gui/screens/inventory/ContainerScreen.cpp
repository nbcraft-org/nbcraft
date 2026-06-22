#include <cfloat>

#include "client/gui/screens/inventory/ContainerScreen.hpp"
#include "client/locale/Language.hpp"
#include "client/renderer/Lighting.hpp"
#include "client/renderer/entity/ItemRenderer.hpp"
#include "renderer/ShaderConstants.hpp"

#define C_TOUCH_STACK_SPLIT_START_TIME 250 // 0.25s

ContainerScreen::ContainerScreen(ContainerMenu* menu)
    : m_pMenu(menu)
    , m_imageWidth(176)
    , m_imageHeight(166)
    , m_leftPos(0)
    , m_topPos(0)
    , m_bSplitStackThisTick(false)
    , m_slotDragStartTime(0)
{
    m_screenType = SCREEN_UNIVERSAL;
    m_bRenderPointer = true;
}

void ContainerScreen::_renderSlot(Slot& slot)
{
    const SlotDisplay& display = getSlotDisplay(slot);

    if (!display.bVisible) return;

    if (display.bIconHolder)
    {
        MatrixStack::Ref matrix = MatrixStack::World.push();
        float off = 3 * display.size / 50.0f;
        matrix->translate(Vec3(-off, -off, 0.0f));
        blitSprite(*m_pMinecraft->m_pTextures, "gui/console/Graphics/IconHolder.png", display.x, display.y, display.size, display.size);
    }
    MatrixStack::Ref matrix = MatrixStack::World.push();
    matrix->translate(Vec3(display.x, display.y, 0));
    matrix->scale(display.size / 18.0f);
    ItemStack& item = slot.getItem();
    if (item.isEmpty())
    {
        const std::string& noItemSprite = display.noItemSprite;

        if (!noItemSprite.empty())
        {
            blitSprite(*m_pMinecraft->m_pTextures, noItemSprite, 0, 0, 16, 16);
            return;
        }

        int icon = display.noItemIcon;
        if (icon >= 0)
        {
            m_pMinecraft->m_pTextures->loadAndBindTexture(C_ITEMS_NAME);
            blit(0, 0, (icon % 16) * 16, (icon / 16) * 16, 16, 16, 0, 0);
        }

        return;
    }
    ItemRenderer::singleton().renderGuiItem(*m_pMinecraft, item, 0, 0, true);
    ItemRenderer::singleton().renderGuiItemOverlay(*m_pMinecraft, item, 0, 0);
}

Slot* ContainerScreen::_findSlot()
{
    return _findSlot(m_menuPointer.x, m_menuPointer.y);
}

Slot* ContainerScreen::_findSlot(int mouseX, int mouseY)
{
    if (!m_pMenu)
        return nullptr;

    for (std::vector<Slot*>::iterator it = m_pMenu->m_slots.begin(); it != m_pMenu->m_slots.end(); ++it)
    {
        Slot* slot = *it;
        if (_isHovering(*slot)) return slot;
    }
    return nullptr;
}

bool ContainerScreen::_isHovering(const Slot& slot) const
{
    return _isHovering(slot, m_menuPointer.x, m_menuPointer.y);
}

bool ContainerScreen::_isHovering(const Slot& slot, int mouseX, int mouseY) const
{
    const SlotDisplay& display = getSlotDisplay(slot);
    if (!display.bVisible) return false;
    mouseX -= m_leftPos;
    mouseY -= m_topPos;
    float off = 3 * display.size / 50.0f;
    return mouseX >= display.x - off && mouseX < display.x + display.size - off && mouseY >= display.y - off && mouseY < display.y + display.size - off;
}

void ContainerScreen::_renderContent(float partialTick)
{
    MatrixStack::Ref matrix = MatrixStack::World.push();
    matrix->translate(Vec3(m_leftPos, m_topPos, 0));

    _renderLabels();

    currentShaderColor = Color::WHITE;

    Slot* hoveredSlot = nullptr;

    for (std::vector<Slot*>::iterator it = m_pMenu->m_slots.begin(); it != m_pMenu->m_slots.end(); ++it)
    {
        Slot* slot = *it;
        _renderSlot(*slot);
        if (_isHovering(*slot))
        {
            const SlotDisplay& display = getSlotDisplay(*slot);
            hoveredSlot = slot;
            //@NOTE: fillGradient is being used instead of fill, so the shader color won't be changed, I think the same happened on the original
            MatrixStack::Ref highlightMatrix = MatrixStack::World.push();
            highlightMatrix->translate(Vec3(display.x, display.y, 0));
            highlightMatrix->scale(display.size / 18.0f);
            fillGradient(0, 0, 16, 16, 0x80FFFFFF, 0x80FFFFFF);
        }
    }

    Inventory* inv = m_pMinecraft->m_pLocalPlayer->m_pInventory;
    if (!inv->getCarried().isEmpty())
    {
        matrix->translate(Vec3(0.0f, 0.0f, 200.0f));
        MatrixStack::Ref carriedMatrix = MatrixStack::World.push();
        carriedMatrix->translate(Vec3(m_menuPointer.x - m_leftPos - 8, m_menuPointer.y - m_topPos - 8, 0.0f));
        if (m_uiTheme == UI_CONSOLE)
            carriedMatrix->scale(3.0f); // 54 / 18.0f
        ItemRenderer::singleton().renderGuiItem(*m_pMinecraft, inv->getCarried(), 0, 0, true);
        ItemRenderer::singleton().renderGuiItemOverlay(*m_pMinecraft, inv->getCarried(), 0, 0);
    }

    if (!inv->getCarried() && hoveredSlot && hoveredSlot->hasItem())
    {
        std::string name = Language::get(hoveredSlot->getItem().getDescriptionId() + ".name");
        if (!name.empty())
        {
            int w = m_pFont->width(name);
            int tx = m_menuPointer.x - m_leftPos;
            int ty = m_menuPointer.y - m_topPos;
            if (m_uiTheme == UI_CONSOLE)
            {
                tx += 10;
                ty -= 32;
                blitNineSlice(*m_pMinecraft->m_pTextures, ScreenRenderer::POINTER_TEXT_PANEL_SLICES, tx - 6, ty - 6, w * 2 + 12, 34, 8);
                MatrixStack::Ref tooltipMatrix = MatrixStack::World.push();
                m_pFont->drawScalableShadow(name, tx, ty + 4, -1);
            }
            else
            {
                tx += 12;
                ty -= 12;
                fillGradient(tx - 3, ty - 3, tx + w + 3, ty + 8 + 3, 0xC0000000, 0xC0000000);
                m_pFont->drawShadow(name, tx, ty, -1);
            }
        }
    }
}

void ContainerScreen::_renderFg(float partialTicks)
{
    Screen::render(partialTicks);
}

void ContainerScreen::_playInteractSound()
{
    m_pMinecraft->m_pSoundEngine->playUI(C_SOUND_UI_PRESS);
}

void ContainerScreen::_tryPlayInteractSound()
{
    if (_useController())
        _playInteractSound();
}

void ContainerScreen::_selectSlot(Slot* slot)
{
    if (!slot) return;
    const SlotDisplay& display = getSlotDisplay(*slot);
    if (!display.bVisible) return;
    int off = 3 * display.size / 50;
    handlePointerLocation(m_leftPos + display.x - off + display.size / 2, m_topPos + display.y - off + display.size / 2);
}

bool ContainerScreen::_selectSlotInDirection(AreaNavigation::Direction dir)
{
    AreaNavigation::ID found = SlotNavigation(this).navigateCyclic(dir, m_menuPointer.x, m_menuPointer.y);

    if (found >= 0)
    {
        _selectSlot(m_pMenu->getSlot(found));
        return true;
    }

    return false;
}

void ContainerScreen::_controllerDirectionChanged(GameController::StickID stickId, GameController::StickState stickState)
{
    Screen::_controllerDirectionChanged(stickId, stickState);
    if (stickId == 1 && stickState == GameController::STICK_STATE_NONE)
    {
        _selectSlot(_findSlot());
    }
}

void ContainerScreen::initMenuPointer()
{
    //@NOTE: Calling this as a fallback, if for some reason, there isn't a slot available
    Screen::initMenuPointer();

    if (!_useController()) return;

    for (std::vector<Slot*>::iterator it = m_pMenu->m_slots.begin(); it != m_pMenu->m_slots.end(); ++it)
    {
        Slot* slot = *it;
        //@NOTE: Selects the first hotbar slot
        if (slot->m_id == 0 && m_pMinecraft->m_pLocalPlayer && slot->m_pContainer == m_pMinecraft->m_pLocalPlayer->m_pInventory)
        {
            _selectSlot(slot);
            break;
        }
    }
}

void ContainerScreen::init()
{
    Screen::init();
    m_pMinecraft->m_pLocalPlayer->m_pContainerMenu = m_pMenu;
    m_leftPos = (m_width - m_imageWidth) / 2;
    m_topPos = (m_height - m_imageHeight) / 2;

    m_slotDisplays.clear();
    for (std::vector<Slot*>::iterator it = m_pMenu->m_slots.begin(); it != m_pMenu->m_slots.end(); ++it)
    {
        m_slotDisplays.push_back(_createSlotDisplay(*(*it)));
    }
}

void ContainerScreen::render(float partialTicks)
{
    renderBackground();
    _renderBg(partialTicks);
    _renderFg(partialTicks);
    _renderContent(partialTicks);
}

void ContainerScreen::pointerPressed(const MenuPointer& pointer, MouseButtonType button)
{
    Screen::pointerPressed(pointer, button);

    if (m_pMinecraft->useTouchscreen())
    {
        if (_findSlot())
            m_slotDragStartTime = getTimeMs();
    }
    else
    {
        slotClicked(pointer, button);
    }
}

void ContainerScreen::pointerReleased(const MenuPointer& pointer, MouseButtonType button)
{
    Screen::pointerReleased(pointer, button);

    if (m_pMinecraft->useTouchscreen())
    {
        if (!m_bSplitStackThisTick)
            slotClicked(pointer, button);
        m_slotDragStartTime = 0.0f;
        m_bSplitStackThisTick = false;
    }
}

void ContainerScreen::handlePointerPressed(bool isPressed)
{
    Screen::handlePointerPressed(isPressed);

    if (m_pMinecraft->useTouchscreen() && isPressed && m_slotDragStartTime > 0.0f)
    {
        float timeMs = getTimeMs();
        float timeSinceDragStart = timeMs - m_slotDragStartTime;
        if (timeSinceDragStart >= C_TOUCH_STACK_SPLIT_START_TIME)
        {
            slotClicked(m_menuPointer, MOUSE_BUTTON_RIGHT);
            m_bSplitStackThisTick = true;
            // reset timer to now
            m_slotDragStartTime = timeMs;
        }
    }
}

void ContainerScreen::slotClicked(const MenuPointer& pointer, MouseButtonType button, bool quick)
{
    if (button == MOUSE_BUTTON_LEFT || button == MOUSE_BUTTON_RIGHT)
    {
        Slot* slot = _findSlot(pointer.x, pointer.y);
        bool outside = pointer.x < m_leftPos || pointer.y < m_topPos || pointer.x >= m_leftPos + m_imageWidth || pointer.y >= m_topPos + m_imageHeight;
        Container::SlotID slotId = -1;
        if (slot) slotId = slot->m_id;
        if (outside) slotId = -999;
        if (slotId != -1)
            slotClicked(slot, slotId, button, slotId != -999 && quick);
    }
}

void ContainerScreen::slotClicked(Slot* slot, Container::SlotID slotId, MouseButtonType button, bool quick)
{
    _tryPlayInteractSound();
    m_pMinecraft->getLocalPlayerGameMode()->handleInventoryMouseClick(m_pMenu->m_containerId, slotId, button, quick, m_pMinecraft->m_pLocalPlayer);
}

void ContainerScreen::slotClicked(const MenuPointer& pointer, MouseButtonType button)
{
    slotClicked(pointer, button, m_pMinecraft->m_pPlatform->shiftPressed());
}

void ContainerScreen::handleUserAction(const ActionInfo& button)
{
    Options& options = *m_pMinecraft->getOptions();
    if (!_useController() && options.isAction(AID_INVENTORY, button))
    {
        m_pMinecraft->handleBack(false);
    }
    else if (options.isAction(AID_CONTAINER_QUICKMOVE, button) && _useController())
    {
        slotClicked(m_menuPointer, MOUSE_BUTTON_LEFT, true);
    }
    else if (options.isAction(AID_CONTAINER_SPLIT, button) && _useController())
    {
        slotClicked(m_menuPointer, MOUSE_BUTTON_RIGHT, false);
    }
    else
    {
        if (_useController() &&
                ((options.isAction(AID_MENU_UP, button) && _selectSlotInDirection(AreaNavigation::UP)) ||
                (options.isAction(AID_MENU_DOWN, button) && _selectSlotInDirection(AreaNavigation::DOWN)) ||
                (options.isAction(AID_MENU_RIGHT, button) && _selectSlotInDirection(AreaNavigation::RIGHT)) ||
                (options.isAction(AID_MENU_LEFT, button) && _selectSlotInDirection(AreaNavigation::LEFT))))
        {
            _playSelectSound();
            return;
        }

        Screen::handleUserAction(button);
    }
}

const SlotDisplay& ContainerScreen::getSlotDisplay(const Slot& slot) const
{
    return m_slotDisplays[slot.m_id];
}

void ContainerScreen::onClose()
{
    if (m_pMinecraft->m_pLocalPlayer)
        m_pMinecraft->m_pLocalPlayer->closeContainer();
}

void ContainerScreen::tick()
{
    Screen::tick();
    if (!m_pMinecraft->m_pLocalPlayer->isAlive() || m_pMinecraft->m_pLocalPlayer->m_bRemoved)
        m_pMinecraft->m_pLocalPlayer->closeContainer();
}

void ContainerScreen::slotsChanged(Container* container)
{
}

bool ContainerScreen::isPauseScreen()
{
    return false;
}

ContainerScreen::SlotNavigation::SlotNavigation(ContainerScreen* screen) : m_pScreen(screen)
{
    m_area.w = screen->m_width;
    m_area.h = screen->m_height;
}

bool ContainerScreen::SlotNavigation::next(int& x, int& y, bool cycle)
{
    while (++m_index < int(m_pScreen->m_slotDisplays.size()))
    {
        const SlotDisplay& display = m_pScreen->m_slotDisplays[m_index];

        if (!display.bVisible || !(cycle || isValid(m_index))) continue;

        int off = 3 * display.size / 50;
        x = m_pScreen->m_leftPos + display.x - off + display.size / 2;
        y = m_pScreen->m_topPos + display.y - off + display.size / 2;

        return true;
    }

    return false;
}

bool ContainerScreen::SlotNavigation::isValid(ID id)
{
    Slot* hovered = m_pScreen->_findSlot();
    return !hovered || hovered->m_id != id;
}
