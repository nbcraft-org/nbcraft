/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include <pthread.h>

#include "CThread_base.hpp"

// pthreads
class CThread : public CThread_base
{
public:
	typedef pthread_id_np_t ID;

public:
	CThread(Function func, void* context)
		: CThread_base(func, context)
	{
		pthread_attr_init(&m_thrd_attr);
		//pthread_attr_setdetachstate(&m_thrd_attr, 1);
		pthread_create(&m_thrd, &m_thrd_attr, m_func, context);
	}
	~CThread()
	{
		join();

		pthread_attr_destroy(&m_thrd_attr);
	}

public:
	void join()
	{
		if (m_bJoined)
			return;

		pthread_join(m_thrd, 0);

		m_bJoined = true;
	}

	static ID GetCurrentThreadId() { return pthread_getthreadid_np(); }

private:
	pthread_t m_thrd;
	pthread_attr_t m_thrd_attr;
};
