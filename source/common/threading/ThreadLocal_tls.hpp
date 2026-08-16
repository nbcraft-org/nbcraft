#pragma once

#include <windows.h>
#include <cassert>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include "common/threading/Mutex.hpp"

// NOTE: TLS does not destruct objects automatically on thread exit
// Please refrain from spawning random threads that call ThreadLocal
template<typename T>
class ThreadLocal
{
public:
	typedef T* (*CreatorFunction_t)();

private:
	// disable copy constructors
	ThreadLocal(const ThreadLocal&);
	ThreadLocal& operator=(const ThreadLocal&);

public:
	ThreadLocal()
		: m_key(TlsAlloc())
		, m_creatorFunction(_Create)
	{
		if (m_key == TLS_OUT_OF_INDEXES)
			throw std::runtime_error("TLS_OUT_OF_INDEXES");
	}

	ThreadLocal(CreatorFunction_t creatorFunction)
		: m_key(TlsAlloc())
		, m_creatorFunction(creatorFunction)
	{
		if (m_key == TLS_OUT_OF_INDEXES)
			throw std::runtime_error("TLS_OUT_OF_INDEXES");
	}

	~ThreadLocal()
	{
		BOOL result = TlsFree(m_key);
		assert(result == TRUE);

		for (typename std::vector<T*>::iterator it = m_pool.begin(); it != m_pool.end(); it++)
			delete (*it);
	}

private:
	T* _get() const
	{
		return reinterpret_cast<T*>(TlsGetValue(m_key));
	}

public:
	T* getLocalPtr()
	{
		T* storedPtr = _get();
		if (storedPtr)
			return storedPtr;

		T* ptr = m_creatorFunction();
		BOOL result = TlsSetValue(m_key, ptr);
		if (!result)
		{
			delete ptr;
			throw std::runtime_error("TlsSetValue failed");
		}

		{
			LockGuard<Mutex> lock(m_poolMutex);

			assert(std::find(m_pool.begin(), m_pool.end(), ptr) == m_pool.end());
			m_pool.push_back(ptr);
		}

		return ptr;
	}

	T& getLocal()
	{
		return *getLocalPtr();
	}

	void resetLocal()
	{
		T* storedPtr = _get();
		if (!storedPtr)
			return;

		{
			LockGuard<Mutex> lock(m_poolMutex);

			typename std::vector<T*>::iterator it = std::find(m_pool.begin(), m_pool.end(), storedPtr);
			assert(it != m_pool.end());
			m_pool.erase(it);
		}

		delete storedPtr;
		TlsSetValue(m_key, nullptr);
	}

private:
	static T* _Create()
	{
		return new T();
	}

private:
	DWORD m_key;
	CreatorFunction_t m_creatorFunction;
	std::vector<T*> m_pool;
	Mutex m_poolMutex;
};
