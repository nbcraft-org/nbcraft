#pragma once

#include "compat/LimitMacros.h"

typedef uint64_t Tick_t;

class Tick
{
public:
	static const Tick_t MAX = UINT64_MAX;
};
