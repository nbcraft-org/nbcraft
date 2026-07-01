/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "../Screen.hpp"

#ifdef ENH_IMPROVED_SAVING

class Entity;

class SavingWorldScreen_Console : public Screen
{
public:
	SavingWorldScreen_Console(bool bCopyMap/*, Entity* pEntityToDeleteAfterSave*/);

	void render(float f) override;
	void tick() override;

public:
	bool m_bCopyMapAtEnd;
	int m_timer;
	//Entity* m_pEntityToDeleteAfterSave;

private:
	std::string m_header;

	std::string m_loadingTip;
	std::vector<std::string> m_loadingTipLines;
};

#endif
