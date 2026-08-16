/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include <stdint.h>

#ifdef _XBOX
#include <xtl.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "CThread_base.hpp"

// Win32
class CThread : public CThread_base
{
public:
	typedef DWORD ID;

public:
	CThread(Function func, void* context)
		: CThread_base(func, context)
	{
		DWORD dwThreadId = 0;
		m_thrd = CreateThread(
			NULL, // not used
			0, // initial stack size
			(LPTHREAD_START_ROUTINE)func, // thread function
			context, // thread argument
			0, // creation option
			&dwThreadId // thread identifier (but does it really matter if I'm the one managing them...?)
		);
	}
	~CThread()
	{
		join();

		CloseHandle(m_thrd);
	}

public:
	void join()
	{
		if (m_bJoined)
			return;

		WaitForSingleObject(m_thrd, INFINITE);

		m_bJoined = true;
	}

	static ID GetCurrentThreadId() { return ::GetCurrentThreadId(); }

private:
	HANDLE m_thrd;
};
