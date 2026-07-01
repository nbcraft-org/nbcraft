/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "BiomeSource.hpp"
#include "world/level/Level.hpp"

BiomeSource::BiomeSource(Level* pLevel) :
	m_temperatureMap(new Random(pLevel->getSeed() * 9871L), 4),
	m_downfallMap(new Random(pLevel->getSeed() * 39811L), 4),
	m_noiseMap(new Random(pLevel->getSeed() * 543321L), 2)
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

const std::vector<Biome*>& BiomeSource::getBiomeBlock(const TilePos& pos, int c, int d)
{
	return getBiomeBlock(m_biomes, pos, c, d);
}

const std::vector<Biome*>& BiomeSource::getBiomeBlock(std::vector<Biome*>& biomes, const TilePos& pos, int c, int d)
{
	int size = c * d;
	if (biomes.size() < size) biomes.resize(size);
	m_temperatureMap.getBiomeRegion(m_temperatures, pos.x, pos.z, c, c, 0.025f, 0.025f, 0.25f);
	m_downfallMap.getBiomeRegion(m_downfalls, pos.x, pos.z, c, c, 0.05f, 0.05f, 1 / 3.0f);
	m_noiseMap.getBiomeRegion(m_noises, pos.x, pos.z, c, c, 0.25f, 0.25f, 0.5882352941176471f);

	int index = 0;

	for (int i = 0; i < c; i++)
	{
		for (int j = 0; j < d; j++)
		{
			float d = m_noises[index] * 1.1 + 0.5;
			float d1 = 0.01f;
			float d2 = 1.0f - d1;
			float d3 = (m_temperatures[index] * 0.15 + 0.7) * d2 + d * d1;
			d1 = 0.002;
			d2 = 1.0 - d1;
			float d4 = (m_downfalls[index] * 0.15 + 0.5) * d2 + d * d1;
			d3 = 1.0 - (1.0 - d3) * (1.0 - d3);

			if (d3 < 0.0) d3 = 0.0;
			if (d4 < 0.0) d4 = 0.0;
			if (d3 > 1.0) d3 = 1.0;
			if (d4 > 1.0) d4 = 1.0;

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
	m_noiseMap.getBiomeRegion(m_noises, a, b, c, d, 0.25f, 0.25f, 0.5882352941176471);

	int index = 0;

	for (int i = 0; i < c; i++)
	{
		for (int j = 0; j < d; j++)
		{
			float d = m_noises[index] * 1.1 + 0.5;
			float d1 = 0.01;
			float d2 = 1.0 - d1;
			float d3 = (m_temperatures[index] * 0.15 + 0.7) * d2 + d * d1;
			d3 = 1.0 - (1.0 - d3) * (1.0f - d3);
			if (d3 < 0.0)
				d3 = 0.0;
			if (d3 > 1.0)
				d3 = 1.0;

			m_temperatures[index] = d3;
			index++;
		}
	}

	return m_temperatures;
}

BiomeSource::~BiomeSource()
{

}
