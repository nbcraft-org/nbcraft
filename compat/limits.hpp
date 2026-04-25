#pragma once

#if !defined(__GNUC__) || (__GNUC__ >= 3)

#include <limits>

#else

#include <math.h>
#include <float.h>

namespace std
{
    template <class T>
    struct numeric_limits
    {
        enum { is_specialized = 0 };
        enum { is_signed = 0 };
        enum { is_integer = 0 };
        enum { is_iec559 = 0 };
    };

    template <>
    struct numeric_limits<float>
    {
        enum { is_specialized = 1 };
        enum { is_signed = 1 };
        enum { is_integer = 0 };
        enum { is_iec559 = 1 };

        static float infinity()
        {
            return (float)HUGE_VAL;
        }

        static float max()
        {
            return FLT_MAX;
        }

        static float min()
        {
            return FLT_MIN;
        }
    };

    template <>
    struct numeric_limits<double>
    {
        enum { is_specialized = 1 };
        enum { is_signed = 1 };
        enum { is_integer = 0 };
        enum { is_iec559 = 1 };

        static double infinity()
        {
            return HUGE_VAL;
        }

        static double quiet_NaN()
        {
            return HUGE_VAL - HUGE_VAL;
        }

        static double max()
        {
            return DBL_MAX;
        }

        static double min()
        {
            return DBL_MIN;
        }
    };

    template <>
    struct numeric_limits<long double>
    {
        enum { is_specialized = 1 };
        enum { is_signed = 1 };
        enum { is_integer = 0 };
        enum { is_iec559 = 1 };

        static long double infinity()
        {
            return (long double)HUGE_VAL;
        }

        static long double max()
        {
            return LDBL_MAX;
        }

        static long double min()
        {
            return LDBL_MIN;
        }
    };
}

#endif
