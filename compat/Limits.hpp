#pragma once

#if defined(__GNUC__) && __GNUC__ < 3

/* GCC before 3.0 didn't have <limits> */

#include "Limits_Legacy.hpp"

#else

#include <limits>

#endif
