/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "SavingWorldScreen_Console.hpp"
#include "RenameMPLevelScreen.hpp"
#include "StartMenuScreen.hpp"
#include "client/resources/LoadingTipManager.hpp"
#include "client/locale/Language.hpp"
#include "client/renderer/LogoRenderer.hpp"

#ifdef ENH_IMPROVED_SAVING

SavingWorldScreen_Console::SavingWorldScreen_Console(bool bCopyMap/*, Entity* pEnt*/)
{
	m_bCopyMapAtEnd = bCopyMap;
	m_timer = 0;
	//m_pEntityToDeleteAfterSave = pEnt;
	m_uiTheme = UI_CONSOLE;
	Screen::init();

	m_header = "Preparing to Save Level";
}

void SavingWorldScreen_Console::render(float f)
{
	renderMenuBackground(f);

	int loadingBarX = m_width / 2 - 320;
	int loadingBarY = m_height / 2 + 30;
	m_pFont->drawScalableShadow("Preparing Chunks...", loadingBarX + 7, loadingBarY - 15, Color::WHITE, 1.5f);
	blitTexture(*m_pMinecraft->m_pTextures, "gui/loading_background.png", loadingBarX, loadingBarY, 0, 0, 640, 20);

	float prog = Mth::clamp(m_pMinecraft->m_progressPercent / 100.0f, 0.0f, 1.0f);

	if (prog >= 0)
		blitTexture(*m_pMinecraft->m_pTextures, "gui/loading_bar.png", loadingBarX + 2, loadingBarY + 2, 0, 0, int(636 * Mth::clamp(prog, 0.0f, 1.0f)), 16, 636, 16);

	const LoadingTip& tip = LoadingTipManager::singleton().getActual();
	if (!tip.text.empty())
	{
		int panelWidth = 801;
		int panelHeight = 81;
		int loadingBarBottom = loadingBarY + 20;
		int panelY = loadingBarBottom + ((m_height - loadingBarBottom) - panelHeight) / 2;

		blitNineSlice(*m_pMinecraft->m_pTextures, ScreenRenderer::POINTER_TEXT_PANEL_SLICES, (m_width - panelWidth) / 2, panelY, panelWidth, panelHeight, 8);

		if (m_loadingTip != tip.text)
		{
			m_loadingTip = tip.text;
			m_loadingTipLines = m_pFont->split(tip.text, (panelWidth - 60) / 2);
		}

		for (std::vector<std::string>::iterator it = m_loadingTipLines.begin(); it != m_loadingTipLines.end(); ++it)
		{
			std::string& line = *it;

			int lineWidth = m_pFont->width(line);

			m_pFont->drawScalable(line, m_width / 2 - lineWidth, panelY + 11, Color::WHITE);

			panelY += 24;
		}
	}

	m_pFont->drawOutlinedString(m_header, (m_width - m_pFont->width(m_header) * 4) / 2, loadingBarY - 105, Color::WHITE, Color::BLACK);

	LogoRenderer::singleton().render(f);
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
		if (pLevel) pLevel->saveUnsavedChunks();
	}

	if (m_timer == 10)
	{
		m_pMinecraft->m_progressPercent = 66;
		if (pLevel) pLevel->saveLevelData();
	}

	if (m_timer == 15)
	{
		m_pMinecraft->m_progressPercent = 100;
		if (pLevel)
		{
			pLevel->savePlayerData();

			LevelStorage* pStorage = pLevel->getLevelStorage();
			SAFE_DELETE(pStorage);
			SAFE_DELETE(pLevel);

			m_pMinecraft->m_pLevel = nullptr;
		}

		// this is safe to do, since on destruction, nothing accesses the parent level or anything
		//SAFE_DELETE(m_pEntityToDeleteAfterSave);
		// already done by the Level

		m_pMinecraft->m_pCameraEntity = m_pMinecraft->m_pLocalPlayer = nullptr;


		m_pMinecraft->m_bUsingScreen = true;

		if (m_bCopyMapAtEnd)
			m_pMinecraft->setScreen(new RenameMPLevelScreen("_LastJoinedServer"));
		else
			m_pMinecraft->gotoMainMenu();

		m_pMinecraft->m_bUsingScreen = false;

		m_pMinecraft->m_bIsGamePaused = false;

		m_timer = -1;
	}
}

#endif
