#pragma once

#ifdef _XBOX
#include <xtl.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <cassert>
#include <stdexcept>

template<typename T>
class ThreadLocal
{
public:
	typedef T* (*CreatorFunction_t)();

private:
	// disable copy constructors
	ThreadLocal(const ThreadLocal&);
	ThreadLocal& operator=(const ThreadLocal&);

private:
	static T* _Create()
	{
		return new T();
	}

	static void _Destroy(PVOID lpFlsData)
	{
		delete reinterpret_cast<T*>(lpFlsData);
	}

public:
	ThreadLocal()
		: m_key(FlsAlloc(_Destroy))
		, m_creatorFunction(_Create)
	{
		if (m_key == FLS_OUT_OF_INDEXES)
			throw std::runtime_error("FLS_OUT_OF_INDEXES");
	}

	ThreadLocal(CreatorFunction_t creatorFunction)
		: m_key(FlsAlloc(_Destroy))
		, m_creatorFunction(creatorFunction)
	{
		if (m_key == FLS_OUT_OF_INDEXES)
			throw std::runtime_error("FLS_OUT_OF_INDEXES");
	}

	~ThreadLocal()
	{
		BOOL result = FlsFree(m_key);
		assert(result == TRUE);
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
		BOOL result = FlsSetValue(m_key, ptr);
		if (!result)
		{
			delete ptr;
			throw std::runtime_error("FlsSetValue failed");
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

		TlsSetValue(m_key, nullptr);
	}

private:
	DWORD m_key;
	CreatorFunction_t m_creatorFunction;
};
