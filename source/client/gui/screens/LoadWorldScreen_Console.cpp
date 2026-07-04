#include "LoadWorldScreen_Console.hpp"
#include "common/Util.hpp"
#include "client/locale/Language.hpp"

LoadWorldScreen_Console::LoadWorldScreen_Console(Options& options, Screen* parent, const LevelSummary& summary) 
	: PanelScreen_Console(parent)
	, m_onlineGame(0, 0, true, Language::get("loadMenu.online"))
	, m_inviteOnly(0, 0, false, Language::get("loadMenu.inviteOnly"))
	, m_difficultySlider(0, 0, 400, 32, &options.m_difficulty, options.m_difficulty.getMessage(), options.m_difficulty.toFloat())
	, m_btnLoad(0, 0, 400, 40, Language::get("loadMenu.load"))
	, m_summary(summary)
{
	m_onlineGame.setEnabled(false);
	m_inviteOnly.setEnabled(false);
}

void LoadWorldScreen_Console::_buttonClicked(Button* pButton)
{
	if (pButton->getId() == m_btnLoad.getId())
	{
		m_pMinecraft->selectLevel(m_summary);
	}
}

void LoadWorldScreen_Console::init()
{
	m_panel.w = 450;
	m_panel.h = 360;
	m_panel.x = (m_width - m_panel.w) / 2;
	m_panel.y = (m_height - m_panel.h) / 2 + 70;

	int left = m_panel.x + 25;

	m_onlineGame.m_xPos = left;
	m_onlineGame.m_yPos = m_panel.y + 130;
	_addElement(m_onlineGame);

	m_inviteOnly.m_xPos = left;
	m_inviteOnly.m_yPos = m_onlineGame.m_yPos + 36;
	_addElement(m_inviteOnly);

	m_difficultySlider.m_xPos = left;
	m_difficultySlider.m_yPos = m_onlineGame.m_yPos + 116;
	_addElement(m_difficultySlider);

	m_btnLoad.m_xPos = left;
	m_btnLoad.m_yPos = m_difficultySlider.m_yPos + 50;
	_addElement(m_btnLoad);
}

void LoadWorldScreen_Console::render(float f)
{
	PanelScreen_Console::render(f);
	Font& font = *m_pFont;

	std::string gamemodeString = Util::format(Language::get("loadMenu.gamemodeInfo").c_str(), GameTypeConv::GameTypeToNonLocString(m_summary.m_gameType).c_str());
	std::string seedString = Util::format(Language::get("loadMenu.seed").c_str(), Util::toString(m_summary.m_levelSeed).c_str());

	blitNineSlice(*m_pMinecraft->m_pTextures, ScreenRenderer::PANEL_SQUARE_RECESS_SLICES, m_panel.x + 25, m_panel.y + 18, 64, 64, 16);
	blitTexture(*m_pMinecraft->m_pTextures, "pack.png", m_panel.x + 28, m_panel.y + 21, 0, 0, 58, 58);

	font.drawScalable(m_summary.m_levelName, m_panel.x + 100, m_panel.y + 25, Color::TEXT_GREY);
	font.drawScalable(gamemodeString, m_panel.x + 100, m_panel.y + 60, Color::TEXT_GREY);
	font.drawScalable(seedString, m_panel.x + 28, m_panel.y + 100, Color::TEXT_GREY);
}

void LoadWorldScreen_Console::renderPanel(float f)
{
	blitNineSlice(*m_pMinecraft->m_pTextures, ScreenRenderer::PANEL_SLICES, m_panel.x, m_panel.y, m_panel.w, m_panel.h, 32);
}
