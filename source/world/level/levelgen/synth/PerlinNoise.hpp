/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "ImprovedNoise.hpp"

class PerlinNoise : public Synth
{
public:
	PerlinNoise(int nOctaves);
	PerlinNoise(Random*, int nOctaves);
	virtual ~PerlinNoise();
	void init(Random* pRandom, int nOctaves);

	float getValue(float, float) override;
	float getValue(float, float, float);

	const std::vector<float>& getRegion(std::vector<float>& a2, int a3, int a4, int a5, int a6, float a7, float a8, float a9)
	{
		return getRegion(a2, float(a3), 10.0, float(a4), a5, 1, a6, a7, 1.0, a8);
	}

	const std::vector<float>& getRegion(std::vector<float>&, float, float, float, int, int, int, float, float, float);

	const std::vector<float>& getBiomeRegion(std::vector<float>& pMem, float a3, float a4, int a5, int a6, float a7, float a8, float a9)
	{
		return getBiomeRegion(pMem, a3, a4, a5, a6, a7, a8, a9, 0.5);
	}

	const std::vector<float>& getBiomeRegion(std::vector<float>& pMem, float a3, float a4, int a5, int a6, float a7, float a8, float a9, float a10);

private:
	ImprovedNoise** m_pImprovedNoise;
	int m_nOctaves;
};

