#pragma once

#ifdef _WIN32

#ifndef USE_WIN32_THREADS
#if defined(_XBOX) || defined(USE_OLD_CPP) || defined(__MINGW32__)
// USE_WIN32_THREADS - Use a Win32 implementation of threads instead of using pthread
#define USE_WIN32_THREADS
#else
// USE_CPP11_THREADS - Use a C++11 implementation of threads instead of using pthread
#define USE_CPP11_THREADS
#endif
#endif

#else // !defined(_WIN32)

#define USE_PTHREADS

#endif

#ifdef USE_CPP11_THREADS

// C++11 STL
#include "CThread_std.hpp"

#elif defined(USE_WIN32_THREADS)

// Win32
#include "CThread_win32.hpp"

#elif defined(XENON)

// libxenon
#include "CThread_xenon.hpp"

#elif defined(USE_PTHREADS)

// pthreads
#include "CThread_pthread.hpp"

#endif
