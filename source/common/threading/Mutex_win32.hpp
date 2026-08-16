#pragma once

#ifdef _XBOX
#include <xtl.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <cassert>

#ifdef _DEBUG

// Enables recursion detection
// Ensures that Mutex is always used in a non-recursive way on Windows
// This can be used to prevent deadlocks on other systems
// as critical sections on Windows are recursion safe
#define _MUTEX_DEBUG_RECURSION_DETECTION_

#endif

class Mutex
{
private:
	// disable copy constructors
	Mutex(const Mutex&);
	Mutex& operator=(const Mutex&);

public:
	Mutex()
	{
		InitializeCriticalSection(&m_criticalSection);

#ifdef _MUTEX_DEBUG_RECURSION_DETECTION_
		m_owningThreadId = 0;
#endif
	}

	~Mutex()
	{
		DeleteCriticalSection(&m_criticalSection);
	}

public:
	void lock()
	{
#ifdef _MUTEX_DEBUG_RECURSION_DETECTION_
		if (m_owningThreadId == GetCurrentThreadId())
		{
			// Recursion is now allowed with regular mutexes
			DebugBreak();
		}
#endif

		EnterCriticalSection(&m_criticalSection);

#ifdef _MUTEX_DEBUG_RECURSION_DETECTION_
		m_owningThreadId = GetCurrentThreadId();
#endif
	}

	void unlock()
	{
#ifdef _MUTEX_DEBUG_RECURSION_DETECTION_
		if (m_owningThreadId == GetCurrentThreadId())
			m_owningThreadId = 0;
#endif

		LeaveCriticalSection(&m_criticalSection);
	}

private:
	CRITICAL_SECTION m_criticalSection;
#ifdef _MUTEX_DEBUG_RECURSION_DETECTION_
	DWORD m_owningThreadId;
#endif
};

class RecursiveMutex
{
private:
	// disable copy constructors
	RecursiveMutex(const RecursiveMutex&);
	RecursiveMutex& operator=(const RecursiveMutex&);

public:
	RecursiveMutex()
	{
		InitializeCriticalSection(&m_criticalSection);
	}

	~RecursiveMutex()
	{
		DeleteCriticalSection(&m_criticalSection);
	}

public:
	void lock()
	{
		EnterCriticalSection(&m_criticalSection);
	}

	void unlock()
	{
		LeaveCriticalSection(&m_criticalSection);
	}

private:
	CRITICAL_SECTION m_criticalSection;
};

#ifdef _MUTEX_DEBUG_RECURSION_DETECTION_
#undef _MUTEX_DEBUG_RECURSION_DETECTION_
#endif
