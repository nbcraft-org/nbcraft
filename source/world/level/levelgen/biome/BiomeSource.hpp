/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "Biome.hpp"
#include "world/level/levelgen/chunk/ChunkPos.hpp"
#include "world/level/TilePos.hpp"

class Level;
class BiomeSource
{

public:
	BiomeSource() : 
		m_temperatureMap(0), 
		m_downfallMap(0), 
		m_noiseMap(0) 
	{ }
	BiomeSource(Level*);
	float getTemperature(int x, int y);
	virtual ~BiomeSource();

	virtual Biome* getBiome(const ChunkPos& pos);
	virtual Biome* getBiomeAt(const TilePos& pos);
	virtual const std::vector<Biome*>& getBiomeBlock(const TilePos& pos, int, int);
	virtual const std::vector<Biome*>& getBiomeBlock(std::vector<Biome*>&, const TilePos& pos, int, int);
	virtual const std::vector<float>& getTemperatureBlock(int, int, int, int);

public:
	std::vector<float> m_temperatures;
	std::vector<float> m_downfalls;
	std::vector<float> m_noises;
	std::vector<Biome*> m_biomes;
	PerlinNoise m_temperatureMap;
	PerlinNoise m_downfallMap;
	PerlinNoise m_noiseMap;
};

