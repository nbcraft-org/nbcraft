/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include <stdint.h>
#include "common/Utils.hpp"

// CThread - Object oriented pthread wrapper
class CThread_base
{
public:
	typedef void* (*Function)(void*);

public:
	CThread_base(Function func, void* context)
		: m_bJoined(false)
		, m_func(func)
	{
	}

public:
	void join()
	{
		if (m_bJoined)
			return;

		m_bJoined = true;
	}

	static void sleep(uint32_t ms) { sleepMs(ms); }

protected:
	bool m_bJoined;
	Function m_func;
};
