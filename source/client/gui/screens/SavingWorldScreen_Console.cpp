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
	m_subtext = Language::get("saving.progress");
}

void SavingWorldScreen_Console::tick()
{
	Level* pLevel = m_pMinecraft->m_pLevel;

	if (m_timer < 0)
		return;

	m_timer++;

	if (m_timer == 5)
	{
		m_pMinecraft->m_progressPercent = 33;
		
	}

	if (m_timer == 10)
	{
		m_pMinecraft->m_progressPercent = 66;
	}

	if (m_timer == 15)
	{
		m_pMinecraft->m_progressPercent = 100;

		m_pMinecraft->unloadLevel(m_bCopyMapAtEnd);

		m_timer = -1;
	}
}

void SavingWorldScreen_Console::updateEvents()
{
	Screen::updateEvents();
}

#endif
