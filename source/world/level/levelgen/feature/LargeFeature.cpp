#include "LargeFeature.hpp"
#include "world/level/TileSource.hpp"

LargeFeature::LargeFeature()
{
	m_radius = 8;
}

LargeFeature::~LargeFeature()
{
}

void LargeFeature::apply(ChunkSource* csrc, TileSource& source, int x, int z, uint8_t* tiles, int unk)
{
	Level& level = source.getLevel();

	m_random.setSeed(level.getSeed());

	int r1 = 2 * (m_random.nextInt() / 2) + 1;
	int r2 = 2 * (m_random.nextInt() / 2) + 1;

	for (int ax = x - m_radius; ax <= x + m_radius; ax++)
	{
		for (int az = z - m_radius; az <= z + m_radius; az++)
		{
			m_random.setSeed((r1 * ax + r2 * az) & level.getSeed());
			addFeature(source, ax, az, x, z, tiles, unk);
		}
	}
}
