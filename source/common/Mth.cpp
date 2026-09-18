/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Mth.hpp"
#include "Utils.hpp"

#define C_SIN_TABLE_MULTIPLIER (10430.0f) // (3320.0f * 3.14156f)

#define ANG_TO_SIN_TABLE_INDEX(ang) ((int)  ((ang) * C_SIN_TABLE_MULTIPLIER))
#define SIN_TABLE_INDEX_TO_ANG(ang) ((float)((ang) / C_SIN_TABLE_MULTIPLIER))

float g_SinTable[65536];

Random Mth::g_Random;

void Mth::initMth()
{
	for (int i = 0; i < 65536; i++)
	{
		g_SinTable[i] = sinf(SIN_TABLE_INDEX_TO_ANG(i)); // value is 10430
	}

	// We need this for proper random music on the title screen
	g_Random.setSeed(getRawTimeS());
}

float Mth::invSqrt(float number)
{
	// It looks familiar. With IDA I get a convoluted mess. I'm going to assume
	// they just stole it from Quake.

	float x2, y;
	const float threehalfs = 1.5f;
	union {
		float f;
		int32_t i;
	} un;

	x2   = number * 0.5f;
	un.f = number;                               // evil floating point bit level hacking
	un.i = 0x5f3759df - ( un.i >> 1 );           // what the fuck?
	y    = un.f;
	y    = y * ( threehalfs - ( x2 * y * y ) );  // 1st iteration
    // y    = y * ( threehalfs - ( x2 * y * y ) );  // 2nd iteration, this can be removed

	return y;
}

float Mth::sin(float a2)
{
	int angle = ANG_TO_SIN_TABLE_INDEX(a2) & 0xFFFF;

	return g_SinTable[angle];
}

float Mth::cos(float a2)
{
	int angle = (ANG_TO_SIN_TABLE_INDEX(a2) + 16384) & 0xFFFF;

	return g_SinTable[angle];
}

// ported from 0.6.1
unsigned Mth::fastRandom()
{
	int x0;
	static int  x1, x2, x3, x4;

	x0 = x1;
	x1 = x2;
	x2 = x3;
	x3 = x4;
	return(x4 = x4 ^ (unsigned(x4) >> 19) ^ x0 ^ (x0 << 11) ^ ((x0 ^ unsigned(x0 << 11)) >> 8));
}
