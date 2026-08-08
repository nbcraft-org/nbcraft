/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "Biome.hpp"
#include "common/Random.hpp"
#include "world/level/levelgen/chunk/ChunkPos.hpp"
#include "world/level/TilePos.hpp"

class Level;
class BiomeSource
{

public:
	BiomeSource()
		: m_temperatureMap(0)
		, m_downfallMap(0)
		, m_noiseMap(0)
	{ }
	BiomeSource(Level*);
	float getTemperature(int x, int y);
	virtual ~BiomeSource();

	virtual Biome* getBiome(const ChunkPos& pos);
	virtual Biome* getBiomeAt(const TilePos& pos);
	virtual const Biome::Vector& getBiomeBlock(const TilePos& pos, int c, int d);
	virtual const Biome::Vector& getBiomeBlock(Biome::Vector& biomes, const TilePos& pos, int c, int d);
	virtual const std::vector<float>& getTemperatureBlock(int a, int b, int c, int d);

public:
	std::vector<float> m_temperatures;
	std::vector<float> m_downfalls;
	std::vector<float> m_noises;
	Biome::Vector m_biomes;
	Random m_temperatureRandom;
	Random m_downfallRandom;
	Random m_noiseRandom;
	PerlinNoise m_temperatureMap;
	PerlinNoise m_downfallMap;
	PerlinNoise m_noiseMap;
};

