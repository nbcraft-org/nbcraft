/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "PerlinNoise.hpp"
#include "common/Utils.hpp"
#include "common/Logger.hpp"

PerlinNoise::PerlinNoise(int nOctaves)
{
	init(new Random(), nOctaves);
}

PerlinNoise::PerlinNoise(Random* pRandom, int nOctaves)
{
	if (nOctaves == 10)
	{
		LOG_I("PerlinNoise octaves are 10");
	}

	init(pRandom, nOctaves);
}

void PerlinNoise::init(Random* pRandom, int nOctaves)
{
	m_nOctaves = nOctaves;
	m_pImprovedNoise = new ImprovedNoise * [nOctaves];

	for (int i = 0; i < nOctaves; i++)
	{
		m_pImprovedNoise[i] = new ImprovedNoise(pRandom);
	}
}

PerlinNoise::~PerlinNoise()
{
	for (int i = 0; i < m_nOctaves; i++)
		delete m_pImprovedNoise[i];

	delete[] m_pImprovedNoise;
}

float PerlinNoise::getValue(float x, float y)
{
	if (m_nOctaves <= 0) return 0.0f;

	float result = 0.0f, x1 = 1.0f;

	for (int i = 0; i < m_nOctaves; i++)
	{
		result += m_pImprovedNoise[i]->getValue(x * x1, y * x1) / x1;
		x1 /= 2;
	}

	return result;
}

float PerlinNoise::getValue(float x, float y, float z)
{
	if (m_nOctaves <= 0) return 0.0f;

	float result = 0.0f, x1 = 1.0f;

	for (int i = 0; i < m_nOctaves; i++)
	{
		result += m_pImprovedNoise[i]->getValue(x * x1, y * x1, z * x1) / x1;
		x1 /= 2;
	}

	return result;
}

const std::vector<float>& PerlinNoise::getRegion(std::vector<float>& pMem, float a3, float a4, float a5, int a6, int a7, int a8, float a9, float a10, float a11)
{
	pMem.assign(a6 * a7 * a8, 0.0f);
	
	float x = 1.0f;
	for (int i = 0; i < m_nOctaves; i++)
	{
		m_pImprovedNoise[i]->add(pMem, a3, a4, a5, a6, a7, a8, a9 * x, a10 * x, a11 * x, x);
		x /= 2;
	}

	return pMem;
}


const std::vector<float>& PerlinNoise::getBiomeRegion(std::vector<float>& pMem, float a3, float a4, int a5, int a6, float a7, float a8, float a9, float a10)
{
	a7 /= 1.5f;
	a8 /= 1.5f;
	pMem.assign(a5 * a6, 0.0f);

	float x = 1.0f, y = 1.0f;
	for (int i = 0; i < m_nOctaves; i++)
	{
		m_pImprovedNoise[i]->addBiome(pMem, a3, a4, a5, a6, a7 * y, a8 * y, 0.55 / x);
		x *= a10;
		y *= a9;
	}

	return pMem;
}
