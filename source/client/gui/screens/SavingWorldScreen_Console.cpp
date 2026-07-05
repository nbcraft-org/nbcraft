#include "SavingWorldScreen_Console.hpp"
#include "client/locale/Language.hpp"

#ifdef ENH_IMPROVED_SAVING

SavingWorldScreen_Console::SavingWorldScreen_Console(bool bCopyMap/*, Entity* pEnt*/)
{
	m_bCopyMapAtEnd = bCopyMap;
	m_timer = 0;
	//m_pEntityToDeleteAfterSave = pEnt;
	m_uiTheme = UI_CONSOLE;
	
}

void SavingWorldScreen_Console::init()
{
	ProgressScreen_Console::init();

	m_header = Language::get("saving.save");
}

void SavingWorldScreen_Console::tick()
{
	if (m_timer < 0)
		return;

	m_timer++;

	if (m_timer >= 5)
	{
		m_timer = -1;

		m_pMinecraft->unloadLevel(m_bCopyMapAtEnd);
	}
}

void SavingWorldScreen_Console::updateEvents()
{
	Screen::updateEvents();
}

#endif

std::string SavingWorldScreen_Console::updateText()
{
	return Language::get("saving.progress");
}
