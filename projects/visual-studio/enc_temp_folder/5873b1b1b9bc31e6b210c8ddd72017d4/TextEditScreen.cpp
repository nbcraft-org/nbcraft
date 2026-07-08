#include "client/gui/screens/inventory/TextEditScreen.hpp"
#include "client/renderer/tileentity/TileEntityRenderDispatcher.hpp"
#include "compat/KeyCodes.hpp"
#include "renderer/GlobalConstantBuffers.hpp"

#define C_MAX_SIGN_TEXT_LENGTH 15

TextEditScreen::TextEditScreen(SignTileEntity* sign) 
    : m_title("Edit sign message:")
    , m_frame(0)
    , m_line(0)
    , m_pSign(sign)
    , m_btnDone("Done")
{
}

void TextEditScreen::init()
{
    int x = m_width / 2 - 50;
    int y = m_height / 4 + 20;

    VirtualKeyboard keyboard;

    keyboard.rect = IntRectangle(x / Gui::GuiScale, y / Gui::GuiScale, (x + 100) / Gui::GuiScale, (y + 100) / Gui::GuiScale);
    m_pMinecraft->m_pPlatform->showKeyboard(0, keyboard);

    m_btnDone.m_xPos = m_width / 2 - 100;
    m_btnDone.m_yPos = m_height / 4 + 120;
    _addElement(m_btnDone);
}

void TextEditScreen::render(float partialTicks)
{
    renderBackground();
    drawCenteredString(*m_pFont, m_title, m_width / 2, 40, Color::WHITE);
    MatrixStack::Ref mtx = MatrixStack::World.push();
    mtx->translate(Vec3((float)(m_width / 2), 120.0f, 50.0f));
    constexpr float ss = 93.75f;
    mtx->scale(-ss);
    mtx->rotate(180.0f, Vec3::UNIT_Y);
    Tile* tile = m_pSign->getTile();
    float rot = 0.0f;
    if (tile == Tile::sign)
        rot = (float)(m_pSign->getData() * 360) / 16.0f;
    else
    {
        int face = m_pSign->getData();
        if (face == Facing::NORTH)
            rot = 180.0f;
        else if (face == Facing::WEST)
            rot = 90.0f;
        else if (face == Facing::EAST)
            rot = -90.0f;
    }
    mtx->rotate(rot, Vec3::UNIT_Y);
    mtx->translate(Vec3(0.0f, 0.3125f, 0.0f));

    if (m_frame / 6 % 2 == 0)
        m_pSign->m_selectedLine = m_line;

#ifdef FEATURE_GFX_SHADERS
    mce::GlobalConstantBuffers& globalBuffers = mce::GlobalConstantBuffers::getInstance();
    mce::EntityConstants& cEntity = globalBuffers.m_entityConstants;

    Color tileLightColor = Color::WHITE;
    cEntity.TILE_LIGHT_COLOR->setData(&tileLightColor);

    Color overlayColor = Color::NIL;
    cEntity.OVERLAY_COLOR->setData(&overlayColor);

    cEntity.sync();
#endif
    TileEntityRenderDispatcher::getInstance()->render(m_pSign, Vec3(-0.5f, -0.75f, -0.5f), 0.0f);
    m_pSign->m_selectedLine = -1;
    mtx.release();
    Screen::render(partialTicks);
}

void TextEditScreen::handlePointerAction(const MenuPointer& pointer, MouseButtonType button)
{
    Screen::handlePointerAction(pointer, button);
}

void TextEditScreen::handleUserAction(const ActionInfo& action)
{
    Screen::handleUserAction(action);

    Options& opts = *m_pMinecraft->getOptions();

    if (opts.isAction(AID_MENU_UP, action))
        m_line = m_line - 1 & 3;

    if (opts.isAction(AID_MENU_DOWN, action) || action.isKey(AKEYCODE_ENTER))
        m_line = m_line + 1 & 3;

    std::string& line = m_pSign->m_messages[m_line];
    if (action.isKey(AKEYCODE_DEL) && line.length() > 0)
        line.erase(line.end() - 1);
}

void TextEditScreen::handleTextChar(char chr)
{
    Screen::handleTextChar(chr);
    if (chr == '\n' || chr < ' ' || chr > '~')
        return;
    std::string& line = m_pSign->m_messages[m_line];
    if (line.length() < C_MAX_SIGN_TEXT_LENGTH)
        line.insert(line.end(), chr);
}

void TextEditScreen::_buttonClicked(Button* pButton)
{
    if (pButton->isEnabled() && pButton->getId() == m_btnDone.getId())
    {
        m_pSign->setChanged();
        m_pMinecraft->setScreen(nullptr);
    }
}

void TextEditScreen::tick()
{
    ++m_frame;
}

void TextEditScreen::removed()
{
    if (m_pMinecraft->m_pLevel->m_bIsClientSide)
        m_pMinecraft->m_pRakNetInstance->send(m_pSign->getUpdatePacket());
    m_pMinecraft->m_pPlatform->hideKeyboard(0);
}
