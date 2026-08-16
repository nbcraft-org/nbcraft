#pragma once

#include <xenon_soc/xenon_power.h>

#include "CThread_base.hpp"

// libxenon
class CThread : public CThread_base
{
public:
	typedef int ID;

public:
	CThread(Function func, void* context)
		: CThread_base(func, context)
	{
		m_thrd = hardwareThread++;
		xenon_run_thread_task(m_thrd, context, (void*)m_func);

		if (hardwareThread >= 6)
			hardwareThread = 1;
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

		xenon_sleep_thread(m_thrd);

		m_bJoined = true;
	}

	// not clear if this is even a feature of libxenon, their docs are non-existent
	static ID GetCurrentThreadId() { return 0; }

private:
	ID m_thrd;
	
private:
	static volatile ID hardwareThread = 1;
};
