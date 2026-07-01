#include "JoinGameScreen_Console.hpp"
#include "common/Util.hpp"
#include "client/locale/Language.hpp"
#include "renderer/ShaderConstants.hpp"

UsernameButton::UsernameButton(const std::string& text) : Button(0, 0, 400, 60, text)
{
}

void UsernameButton::render(Minecraft* mc, const MenuPointer& pointer)
{
	if (!mc->m_pScreen->doElementTabbing())
		setSelected(isHovered(mc, pointer));

	Textures& texs = *mc->m_pTextures;
	if (!isEnabled())
		currentShaderColor.a *= 0.5f;
	blitSprite(texs, isSelected() && !hasFocus() ? "gui/console/Graphics/ListButton_Over.png" : "gui/console/Graphics/ListButton_Norm.png", m_xPos, m_yPos, m_width, m_height, &m_materials.ui_textured_and_glcolor);

	Color textColor;
	if (isSelected())
	{
		textColor = Color(220, 220, 0, currentShaderColor.a); // 0xDCDC00
	}
	else
		textColor = Color(224, 224, 224, currentShaderColor.a); // 0xE0E0E0U
	renderMessage(*mc->m_pFont, textColor);

	currentShaderColor = Color::WHITE;
}

void UsernameButton::renderMessage(Font& font, const Color& textColor)
{
	font.drawScalableShadow(getMessage(), m_xPos + 10, m_yPos + (m_height - 16) / 2, textColor);
}

JoinGameScreen_Console::JoinGameScreen_Console(Screen* parent, const PingedCompatibleServer& server) :
	PanelScreen_Console(parent),
	m_server(server),
	m_btnLoad(0, 0, 400, 40, "OK"),
	m_btnJoinGame(0, 0, 450, 40, "Join Game"),
	m_btnUsername(server.m_name.C_String()),
	m_panelAlpha(1.0f)
{

}

void JoinGameScreen_Console::_buttonClicked(Button* pButton)
{
	if (pButton->getId() == m_btnJoinGame.getId())
    {
        m_pMinecraft->joinMultiplayer(m_server);
        m_pMinecraft->getScreenChooser()->pushProgressScreen();
    }
}

void JoinGameScreen_Console::init()
{
	m_panel.w = 450;
	m_panel.h = 340;
	m_panel.x = (m_width - m_panel.w) / 2;
	m_panel.y = (m_height - m_panel.h) / 2 + 60;

	int left = m_panel.x + 60;

	m_btnJoinGame.m_xPos = (m_width - m_btnJoinGame.m_width) / 2;
	m_btnJoinGame.m_yPos = m_panel.y - 50;
	_addElement(m_btnJoinGame);

	m_btnUsername.m_xPos = (m_width - m_btnUsername.m_width) / 2;
	m_btnUsername.m_yPos = m_panel.y + 50;
	_addElement(m_btnUsername);
}

void JoinGameScreen_Console::render(float f)
{
	PanelScreen_Console::render(f);
	Font& font = *m_pFont;
}

void JoinGameScreen_Console::renderPanel(float f)
{
	float targetAlpha = m_btnJoinGame.isSelected() ? 0.5f : 1.0f;

	float fadeSpeed = 0.05f;

	if (m_panelAlpha < targetAlpha)
	{
		m_panelAlpha += fadeSpeed;
		if (m_panelAlpha > targetAlpha) m_panelAlpha = targetAlpha;
	}
	else if (m_panelAlpha > targetAlpha)
	{
		m_panelAlpha -= fadeSpeed;
		if (m_panelAlpha < targetAlpha) m_panelAlpha = targetAlpha;
	}

	currentShaderColor.a = m_panelAlpha;

	blitNineSlice(*m_pMinecraft->m_pTextures, ScreenRenderer::SMALL_PANEL_SLICES, m_panel.x, m_panel.y, m_panel.w, m_panel.h, 16);

	currentShaderColor.a = 1.0f;
}
