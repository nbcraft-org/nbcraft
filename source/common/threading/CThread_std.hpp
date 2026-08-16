/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include <stdint.h>
#include <thread>

#include "CThread_base.hpp"

// C++11 STL
class CThread : public CThread_base
{
public:
	typedef std::thread::id ID;

public:
	CThread(Function func, void* context)
		: CThread_base(func, context)
	{
		std::thread thr(func, context);
		m_thrd.swap(thr);
	}
	~CThread()
	{
		join();
	}

public:
	void join()
	{
		if (m_bJoined)
			return;

		m_thrd.join();

		m_bJoined = true;
	}

	static ID GetCurrentThreadId() { return std::this_thread::get_id(); }

private:
	std::thread m_thrd;
};
