/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include <cmath>

#include "compat/LegacyCPP.hpp"
#include "Random.hpp"

// M_PI / 180
#define MTH_DEG_TO_RAD 0.017453f

class Mth
{
	static Random g_Random;

public:
    template <typename T>
	static T Max(T a, T b)
    {
        return a > b ? a : b;
    }
    
    template <typename T>
	static T Min(T a, T b)
    {
        return a < b ? a : b;
    }
    
	static inline float abs(float f)
    {
        /*if (f < 0.0f)
            f = -f;
        return f;*/
        return fabs(f);
    }
    
	static int abs(int d)
    {
        if (d < 0)
            d = -d;
        return d;
    }
    
	static float absMax(float a2, float a3)
    {
        if (a2 < 0.0f)
            a2 = -a2;
        if (a3 < 0.0f)
            a3 = -a3;
        if (a2 <= a3)
            a2 = a3;
        return a2;
    }
    
	static float absMaxSigned(float a2, float a3)
    {
        if (abs(a2) <= abs(a2))
            a2 = a3;
        return a2;
    }
    
	static float atan(float f)
    {
        return atanf(f);
    }
    
	static float atan2(float y, float x)
    {
        return atan2f(y, x);
    }
    
	static float cos(float);
    
    template <typename T>
	static T clamp(T x, T min, T max)
    {
        if (x > max)
            return max;
        if (x > min)
            return x;
        else
            return min;
        return max;
    }
    
	static int floor(float f)
    {
        int result = int(f);
        
        if (result > f)
            result--;
        
        return result;
    }
    
	static int round(float f)
    {
        return floor(f + 0.5f);
    }
    
	static void initMth();
    
	static int intFloorDiv(int a2, int a3)
    {
        if (a2 < 0)
            return ~(~a2 / a3);
        
        return a2 / a3;
    }
    
    static int intCeilDiv(int a, int b)
    {
        return (a + b - 1) / b;
    }
    
	static float invSqrt(float);
    
	static int random(int max)
    {
        return int(g_Random.nextInt(max));
    }
    
	static float random(void)
    {
        return g_Random.nextFloat();
    }
    
	static float sin(float);
	static unsigned fastRandom();

	static inline float sqrt(float f)
	{
		return sqrtf(f);
	}

	template <typename T>
	static int signum(T val)
	{
		return (T(0) < val) - (val < T(0));
	}
    
	static inline CONSTEXPR float Lerp(float a, float b, float progress)
	{
		return a + progress * (b - a);
	}
};

