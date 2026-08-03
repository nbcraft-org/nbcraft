/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "BiomeSource.hpp"
#include "world/level/Level.hpp"

BiomeSource::BiomeSource(Level* pLevel)
	: m_temperatureRandom((int64_t)pLevel->getSeed() * 9871)
	, m_downfallRandom((int64_t)pLevel->getSeed() * 39811)
	, m_noiseRandom((int64_t)pLevel->getSeed() * 543321)
	, m_temperatureMap(&m_temperatureRandom, 4)
	, m_downfallMap(&m_downfallRandom, 4)
	, m_noiseMap(&m_noiseRandom, 2)
{
}

float BiomeSource::getTemperature(int x, int y) {
	m_temperatureMap.getRegion(m_temperatures, x, y, 1, 1, 0.025f, 0.025f, 0.5f);
	return m_temperatures[0];
}

Biome* BiomeSource::getBiome(const ChunkPos& pos)
{
	return getBiomeAt(TilePos(pos, 0));
}

Biome* BiomeSource::getBiomeAt(const TilePos& pos)
{
	return getBiomeBlock(pos, 1, 1)[0];
}

const Biome::Vector& BiomeSource::getBiomeBlock(const TilePos& pos, int c, int d)
{
	return getBiomeBlock(m_biomes, pos, c, d);
}

const Biome::Vector& BiomeSource::getBiomeBlock(Biome::Vector& biomes, const TilePos& pos, int c, int d)
{
	size_t size = c * d;
	if (biomes.size() < size) biomes.resize(size);
	m_temperatureMap.getBiomeRegion(m_temperatures, pos.x, pos.z, c, c, 0.025f, 0.025f, 0.25f);
	// @PARITY-JAVA: Java uses (1.0f / 3.0f) here
	m_downfallMap.getBiomeRegion(m_downfalls, pos.x, pos.z, c, c, 0.05f, 0.05f, 0.3333f);
	// @PARITY-JAVA: Java uses 0.5882352941176471f here
	m_noiseMap.getBiomeRegion(m_noises, pos.x, pos.z, c, c, 0.25f, 0.25f, 0.588f);

	int index = 0;

	for (int i = 0; i < c; i++)
	{
		for (int j = 0; j < d; j++)
		{
			float d = m_noises[index] * 1.1f + 0.5f;
			float d1 = 0.01f;
			float d2 = 1.0f - d1;
			float d3 = (m_temperatures[index] * 0.15f + 0.7f) * d2 + d * d1;
			d1 = 0.002f;
			d2 = 1.0f - d1;
			float d4 = (m_downfalls[index] * 0.15f + 0.5f) * d2 + d * d1;
			d3 = 1.0f - (1.0f - d3) * (1.0f - d3);

			if (d3 < 0.0f) d3 = 0.0f;
			if (d4 < 0.0f) d4 = 0.0f;
			if (d3 > 1.0f) d3 = 1.0f;
			if (d4 > 1.0f) d4 = 1.0f;

			m_temperatures[index] = d3;
			m_downfalls[index] = d4;
			biomes[index++] = Biome::getBiome(d3, d4);
		}
	}

	return biomes;
}

const std::vector<float>& BiomeSource::getTemperatureBlock(int a, int b, int c, int d)
{
	m_temperatureMap.getBiomeRegion(m_temperatures, a, b, c, d, 0.025f, 0.025f, 0.25f);
	// @PARITY-JAVA: Java uses 0.5882352941176471f here
	m_noiseMap.getBiomeRegion(m_noises, a, b, c, d, 0.25f, 0.25f, 0.588f);

	int index = 0;

	for (int i = 0; i < c; i++)
	{
		for (int j = 0; j < d; j++)
		{
			float d = m_noises[index] * 1.1f + 0.5f;
			float d1 = 0.01f;
			float d2 = 1.0f - d1;
			float d3 = (m_temperatures[index] * 0.15f + 0.7f) * d2 + d * d1;
			d3 = 1.0f - (1.0f - d3) * (1.0f - d3);
			if (d3 < 0.0f)
				d3 = 0.0f;
			if (d3 > 1.0f)
				d3 = 1.0f;

			m_temperatures[index] = d3;
			index++;
		}
	}

	return m_temperatures;
}

BiomeSource::~BiomeSource()
{

}
