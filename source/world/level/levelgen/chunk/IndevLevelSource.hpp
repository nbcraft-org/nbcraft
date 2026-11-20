/********************************************************************
	NBCraft

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include <map>
#include <vector>
#include "ChunkSource.hpp"
#include "common/Random.hpp"
#include "world/level/levelgen/IndevSettings.hpp"

class PerlinNoise;

// Finite world generator in the style of Indev (early 2010).
// The whole map (C_MAX_CHUNKS_X*16 x C_MAX_Y x C_MAX_CHUNKS_Z*16) is generated
// in one go the first time a chunk is requested, and then sliced into chunks.
class IndevLevelSource : public ChunkSource
{
public:
	IndevLevelSource(Level* pLevel, int32_t seed, IndevWorldType type, IndevWorldTheme theme);
	~IndevLevelSource();

	int tick() override;
	bool shouldSave() override;
	bool hasChunk(const ChunkPos& pos) override;
	LevelChunk* create(const ChunkPos& pos) override;
	LevelChunk* getChunk(const ChunkPos& pos) override;
	LevelChunk* getChunkDontCreate(const ChunkPos& pos) override;
	std::string gatherStats() override;
	void postProcess(ChunkSource*, const ChunkPos& pos) override;

private:
	static const int WIDTH  = C_MAX_CHUNKS_X * 16;
	static const int LENGTH = C_MAX_CHUNKS_Z * 16;
	static const int DEPTH  = C_MAX_Y;

	inline static int _index(int x, int y, int z) { return (x << 15) | (z << 7) | y; }
	inline bool _inBounds(int x, int y, int z) const { return x >= 0 && y >= 0 && z >= 0 && x < WIDTH && y < DEPTH && z < LENGTH; }
	inline TileID _get(int x, int y, int z) const { return _inBounds(x, y, z) ? m_pMap[_index(x, y, z)] : TILE_AIR; }
	inline void _set(int x, int y, int z, TileID t) { if (_inBounds(x, y, z)) m_pMap[_index(x, y, z)] = t; }

	void _generate();
	void _raise(std::vector<int>& heightMap);
	void _erode(std::vector<int>& heightMap);
	void _soil(std::vector<int>& heightMap);
	void _soilFloating(std::vector<int>& heightMap);
	void _carveCaves();
	void _populateOre(TileID ore, int abundance, int maxY);
	void _flood();
	void _floodFill(int x, int y, int z, TileID liquid);
	void _grow(std::vector<int>& heightMap);
	void _plant(std::vector<int>& heightMap);
	bool _growTree(int x, int y, int z);

	float _distort(PerlinNoise& a, PerlinNoise& b, float x, float z);

private:
	Level* m_pLevel;
	Random m_random;
	int32_t m_seed;
	IndevWorldType m_type;
	IndevWorldTheme m_theme;
	int m_waterLevel;
	TileID m_liquid;
	TileID* m_pMap;
	std::map<int, LevelChunk*> m_chunks;
};
