/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "SavingWorldScreen.hpp"
#include "RenameMPLevelScreen.hpp"
#include "StartMenuScreen.hpp"

#ifdef ENH_IMPROVED_SAVING

SavingWorldScreen::SavingWorldScreen(bool bCopyMap/*, Entity* pEnt*/)
{
	m_bCopyMapAtEnd = bCopyMap;
	m_timer = 0;
	//m_pEntityToDeleteAfterSave = pEnt;
}

void SavingWorldScreen::render(float f)
{
	renderDirtBackground(0);

	int x_width  = int(Minecraft::width  * Gui::GuiScale);
	int x_height = int(Minecraft::height * Gui::GuiScale);
	int yPos = x_height / 2;

	int width = m_pFont->width("Saving chunks");
	m_pFont->drawShadow("Saving chunks", (x_width - width) / 2, yPos, 0xFFFFFF);
}

void SavingWorldScreen::tick()
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

#endif
