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

	std::string text = "This game has a level autosave feature. When you see the icon above displayed, the game is saving your data. \n Please do not turn off your Xbox 360 console while this icon is on-screen.";

	int maxTextWidth = 220;

	m_autosaveLine = m_pFont->split(text, maxTextWidth);
}

void AutosaveWarningScreen_Console::render(float f)
{
	PanelScreen_Console::render(f);
	Font& font = *m_pFont;

	int yOffs = std::sin(getTimeMs() / 210.0f) * 4.0f;

	blitTexture(*m_pMinecraft->m_pTextures, "gui/console/Graphics/SaveChest.png", m_panel.x + 235, m_panel.y + 60, 0, 0, 48, 48);
	blitTexture(*m_pMinecraft->m_pTextures, "gui/console/Graphics/SaveArrow.png", m_panel.x + 235, m_panel.y + 40 + yOffs, 0, 0, 48, 48);

	int textY = m_panel.y + 135;

	for (std::vector<std::string>::iterator it = m_autosaveLine.begin(); it != m_autosaveLine.end(); ++it)
	{
		std::string& line = *it;

		font.drawScalable(line, m_panel.x + 35, textY, Color::TEXT_GREY);

		textY += 26;
	}
}

void AutosaveWarningScreen_Console::renderPanel(float f)
{
	blitNineSlice(*m_pMinecraft->m_pTextures, ScreenRenderer::PANEL_SLICES, m_panel.x, m_panel.y, m_panel.w, m_panel.h, 32);
}
