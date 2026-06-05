#include "DeathScreen_Console.hpp"
#include "client/locale/Language.hpp"

DeathScreen_Console::DeathScreen_Console() :
	m_btnRespawn(0, 0, 400, 40, Language::get("deadMenu.respawn")),
	m_btnTitle(0, 0, 400, 40, Language::get("deadMenu.quit"))
{
	m_uiTheme = UI_CONSOLE;
	m_tickCounter = 0;

	m_header = Language::get("deadMenu.death");
}

void DeathScreen_Console::init()
{
	m_btnRespawn.m_xPos = m_width / 2 - m_btnRespawn.m_width / 2;
	m_btnTitle.m_xPos = m_width / 2 - m_btnTitle.m_width / 2;

	m_btnRespawn.m_yPos = m_height / 2 + 40;
	m_btnTitle.m_yPos = m_btnRespawn.m_yPos + 50;

	_addElement(m_btnRespawn);
	_addElement(m_btnTitle);
}

void DeathScreen_Console::_buttonClicked(Button* pButton)
{
	if (pButton->getId() == m_btnRespawn.getId())
	{
		m_pMinecraft->m_pLocalPlayer->respawn();
		m_pMinecraft->setScreen(nullptr);
	}
	else if (pButton->getId() == m_btnTitle.getId())
	{
		m_pMinecraft->leaveGame(false);
	}
}

void DeathScreen_Console::tick()
{
	m_tickCounter++;
}

void DeathScreen_Console::render(float f)
{
	float fade = float(m_tickCounter) / 30.0f;

	if (fade > 1.0f)
		fade = 1.0f;

	int x = int(fade * fade * 255.0f);

	int topAlpha = int((x / 255.0f) * 220);
	int bkgdColor = (topAlpha << 24) | 0x0C002E;

	fillGradient(0, 0, m_width, m_height, bkgdColor, bkgdColor);
	m_pFont->drawOutlinedString(m_header, (m_width / 2) - ((m_pFont->width(m_header) * 4.5f) / 2), m_height / 4 - 50, Color::WHITE, Color::BLACK, 4.5f);
	
	Screen::render(f);
}
