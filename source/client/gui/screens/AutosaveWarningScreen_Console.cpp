#include "AutosaveWarningScreen_Console.hpp"
#include "common/Util.hpp"
#include "client/locale/Language.hpp"
#include "CreateWorldScreen.hpp"

AutosaveWarningScreen_Console::AutosaveWarningScreen_Console(Screen* parent) :
	PanelScreen_Console(parent),
	m_btnLoad(0, 0, 400, 40, "OK")
{

}

void AutosaveWarningScreen_Console::_buttonClicked(Button* pButton)
{
	if (pButton->getId() == m_btnLoad.getId())
	{
		m_pMinecraft->getScreenChooser()->pushStartScreen();
	}
}

void AutosaveWarningScreen_Console::init()
{
	m_panel.w = 520;
	m_panel.h = 420;
	m_panel.x = (m_width - m_panel.w) / 2;
	m_panel.y = (m_height - m_panel.h) / 2 + 50;

	int left = m_panel.x + 60;


	m_btnLoad.m_xPos = left;
	m_btnLoad.m_yPos = m_panel.y + 340;
	_addElement(m_btnLoad);
}

void AutosaveWarningScreen_Console::render(float f)
{
	PanelScreen_Console::render(f);
	Font& font = *m_pFont;

	int yOffs = std::sin(getTimeMs() / 210.0f) * 4.0f;

	blitTexture(*m_pMinecraft->m_pTextures, "gui/console/Graphics/SaveChest.png", m_panel.x + 235, m_panel.y + 60, 0, 0, 48, 48);
	blitTexture(*m_pMinecraft->m_pTextures, "gui/console/Graphics/SaveArrow.png", m_panel.x + 235, m_panel.y + 40 + yOffs, 0, 0, 48, 48);

	font.drawScalable("This game has a level autosave feature.", m_panel.x + 35, m_panel.y + 135, Color::TEXT_GREY);
	font.drawScalable("When you see the icon above displayed, ", m_panel.x + 35, m_panel.y + 135 + 26, Color::TEXT_GREY);
	font.drawScalable("the game is saving your data.          ", m_panel.x + 35, m_panel.y + 135 + 26 + 26, Color::TEXT_GREY);
	font.drawScalable("Please do not turn off your Xbox 360   ", m_panel.x + 35, m_panel.y + 135 + 26 + 26 + 26, Color::TEXT_GREY);
	font.drawScalable("console while this icon is on-screen.  ", m_panel.x + 35, m_panel.y + 135 + 26 + 26 + 26 + 26, Color::TEXT_GREY);
}

void AutosaveWarningScreen_Console::renderPanel(float f)
{
	blitNineSlice(*m_pMinecraft->m_pTextures, ScreenRenderer::PANEL_SLICES, m_panel.x, m_panel.y, m_panel.w, m_panel.h, 32);
}
