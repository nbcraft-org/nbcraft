#pragma once

#include "ProgressScreen_Console.hpp"

#ifdef ENH_IMPROVED_SAVING

class Entity;

class SavingWorldScreen_Console : public ProgressScreen_Console
{
public:
	SavingWorldScreen_Console(bool bCopyMap/*, Entity* pEntityToDeleteAfterSave*/);

	void tick() override;
	void updateEvents() override;
	void init() override;

public:
	bool m_bCopyMapAtEnd;
	int m_timer;
	//Entity* m_pEntityToDeleteAfterSave;
};

#endif
