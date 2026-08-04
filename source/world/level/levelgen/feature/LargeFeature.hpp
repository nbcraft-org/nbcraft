#pragma once

#include "common/Random.hpp"
#include "world/level/Level.hpp"

class LargeFeature
{
public:
	LargeFeature();
	virtual ~LargeFeature();
	virtual void apply(ChunkSource*, TileSource&, int, int, TileID*, int);
	virtual void addFeature(TileSource&, int, int, int, int, TileID*, int) = 0;


public:
	int m_radius;
	Random m_random;
};

