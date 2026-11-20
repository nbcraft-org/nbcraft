/********************************************************************
	NBCraft

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "IndevLevelSource.hpp"

#include <cstring>
#include "common/Logger.hpp"
#include "common/Mth.hpp"
#include "world/level/Level.hpp"
#include "world/level/levelgen/synth/PerlinNoise.hpp"
#include "LevelChunk.hpp"

IndevLevelSource::IndevLevelSource(Level* pLevel, int32_t seed, IndevWorldType type, IndevWorldTheme theme) :
	m_pLevel(pLevel),
	m_random(seed),
	m_seed(seed),
	m_type(type),
	m_theme(theme),
	m_pMap(nullptr)
{
	m_waterLevel = DEPTH / 2;
	m_liquid = (m_theme == INDEV_THEME_HELL) ? TILE_LAVA_CALM : TILE_WATER_CALM;

	LOG_I("Generating Indev world with seed: %d (type: %s, theme: %s)", seed, Indev::GetTypeName(type), Indev::GetThemeName(theme));
}

IndevLevelSource::~IndevLevelSource()
{
	// Chunks are owned by the ChunkCache once handed out.
	delete[] m_pMap;
}

float IndevLevelSource::_distort(PerlinNoise& a, PerlinNoise& b, float x, float z)
{
	return a.getValue(x + b.getValue(x, z), z);
}

void IndevLevelSource::_generate()
{
	if (m_pMap)
		return;

	m_pMap = new TileID[WIDTH * LENGTH * DEPTH];
	memset(m_pMap, 0, sizeof(TileID) * WIDTH * LENGTH * DEPTH);

	std::vector<int> heightMap(WIDTH * LENGTH, 0);

	LOG_I("Indev: Raising..");
	_raise(heightMap);

	LOG_I("Indev: Eroding..");
	_erode(heightMap);

	LOG_I("Indev: Soiling..");
	if (m_type == INDEV_TYPE_FLOATING)
		_soilFloating(heightMap);
	else
		_soil(heightMap);

	LOG_I("Indev: Carving..");
	_carveCaves();
	_populateOre(TILE_ORE_COAL,    90, DEPTH);
	_populateOre(TILE_ORE_IRON,    70, m_waterLevel + 8);
	_populateOre(TILE_ORE_GOLD,    50, m_waterLevel - 16);
	_populateOre(TILE_ORE_EMERALD, 20, m_waterLevel - 40); // diamond

	LOG_I("Indev: Watering..");
	_flood();

	LOG_I("Indev: Growing..");
	_grow(heightMap);

	LOG_I("Indev: Planting..");
	_plant(heightMap);

	// bedrock floor. Floating worlds have no ground at all.
	if (m_type != INDEV_TYPE_FLOATING)
	{
		for (int x = 0; x < WIDTH; x++)
			for (int z = 0; z < LENGTH; z++)
				_set(x, 0, z, TILE_BEDROCK);
	}

	LOG_I("Indev: Done.");
}

void IndevLevelSource::_raise(std::vector<int>& heightMap)
{
	PerlinNoise n1a(&m_random, 8), n1b(&m_random, 8);
	PerlinNoise n2a(&m_random, 8), n2b(&m_random, 8);
	PerlinNoise n3(&m_random, 6);
	PerlinNoise islandNoise(&m_random, 2);

	const bool island = m_type == INDEV_TYPE_ISLAND || m_type == INDEV_TYPE_FLOATING;

	for (int x = 0; x < WIDTH; x++)
	{
		for (int z = 0; z < LENGTH; z++)
		{
			if (m_type == INDEV_TYPE_FLAT)
			{
				heightMap[x + z * WIDTH] = 0;
				continue;
			}

			float h1 = _distort(n1a, n1b, x * 1.3f, z * 1.3f) / 6.0f - 4.0f;
			float h2 = _distort(n2a, n2b, x * 1.3f, z * 1.3f) / 5.0f + 10.0f - 4.0f;

			if (n3.getValue(float(x), float(z)) / 8.0f > 0.0f)
				h2 = h1;

			float h = Mth::Max(h1, h2) / 2.0f;

			if (island)
			{
				float dx = Mth::abs(float(x) / float(WIDTH  - 1) - 0.5f) * 2.0f;
				float dz = Mth::abs(float(z) / float(LENGTH - 1) - 0.5f) * 2.0f;
				float d = Mth::sqrt(dx * dx + dz * dz) * 1.2f;
				d = Mth::Min(d, islandNoise.getValue(x * 0.05f, z * 0.05f) / 4.0f + 1.0f);
				d = Mth::Max(d, Mth::Max(dx, dz));
				if (d > 1.0f) d = 1.0f;
				if (d < 0.0f) d = 0.0f;
				d *= d;
				h = h * (1.0f - d) - d * 10.0f + 5.0f;
				if (h < 0.0f)
					h -= h * h * 0.2f;
			}
			else if (m_type == INDEV_TYPE_INLAND)
			{
				// inland maps sit mostly above the water line
				h = h * 0.8f + 2.0f;
			}
			else if (h < 0.0f)
			{
				h *= 0.8f;
			}

			heightMap[x + z * WIDTH] = int(h);
		}
	}
}

void IndevLevelSource::_erode(std::vector<int>& heightMap)
{
	if (m_type == INDEV_TYPE_FLAT)
		return;

	PerlinNoise e1a(&m_random, 8), e1b(&m_random, 8);
	PerlinNoise e2a(&m_random, 8), e2b(&m_random, 8);

	for (int x = 0; x < WIDTH; x++)
	{
		for (int z = 0; z < LENGTH; z++)
		{
			float a = _distort(e1a, e1b, float(x << 1), float(z << 1)) / 8.0f;
			int b = _distort(e2a, e2b, float(x << 1), float(z << 1)) > 0.0f ? 1 : 0;
			if (a > 2.0f)
			{
				int& h = heightMap[x + z * WIDTH];
				h = ((h - b) / 2 << 1) + b;
			}
		}
	}
}

void IndevLevelSource::_soil(std::vector<int>& heightMap)
{
	PerlinNoise soilNoise(&m_random, 8);

	for (int x = 0; x < WIDTH; x++)
	{
		for (int z = 0; z < LENGTH; z++)
		{
			int dirtDepth = int(soilNoise.getValue(float(x), float(z)) / 24.0f) - 4;
			int surface = heightMap[x + z * WIDTH] + m_waterLevel;
			int stoneTop = surface + dirtDepth;

			if (surface > DEPTH - 2) surface = DEPTH - 2;
			if (surface < 1) surface = 1;

			heightMap[x + z * WIDTH] = Mth::Max(surface, stoneTop);
			if (heightMap[x + z * WIDTH] > DEPTH - 2)
				heightMap[x + z * WIDTH] = DEPTH - 2;

			for (int y = 0; y < DEPTH; y++)
			{
				TileID tile = TILE_AIR;
				if (y <= surface)
					tile = TILE_DIRT;
				if (y <= stoneTop)
					tile = TILE_STONE;
				_set(x, y, z, tile);
			}
		}
	}
}

void IndevLevelSource::_soilFloating(std::vector<int>& heightMap)
{
	// Floating islands: a couple of layers of islands hanging in the void.
	PerlinNoise soilNoise(&m_random, 8);
	PerlinNoise bottomNoise(&m_random, 8);

	const int layers = 2;
	const int layerHeight = DEPTH / (layers + 1);

	std::vector<int> topMap(WIDTH * LENGTH, -1);

	for (int layer = 0; layer < layers; layer++)
	{
		int base = layerHeight * (layer + 1);
		// mirror every other layer so that they don't overlap perfectly
		const bool mirror = (layer & 1) != 0;

		for (int x = 0; x < WIDTH; x++)
		{
			for (int z = 0; z < LENGTH; z++)
			{
				int sx = mirror ? WIDTH - 1 - x : x;
				int sz = mirror ? LENGTH - 1 - z : z;
				int h = heightMap[sx + sz * WIDTH];
				// the island's edge falls off steeply, so dips inside it still count as land
				if (h < -2)
					continue;
				if (h < 1)
					h = 1;

				int dirtDepth = int(soilNoise.getValue(float(x), float(z + layer * 300)) / 24.0f) - 4;
				int bottomDepth = int(Mth::abs(bottomNoise.getValue(float(x), float(z + layer * 300))) / 6.0f) + h * 2;

				int surface = base + h;
				int stoneTop = surface + dirtDepth;
				int bottom = base - bottomDepth;

				if (surface > DEPTH - 2) surface = DEPTH - 2;
				if (bottom < 1) bottom = 1;

				for (int y = bottom; y <= surface; y++)
				{
					TileID tile = (y <= stoneTop) ? TILE_STONE : TILE_DIRT;
					_set(x, y, z, tile);
				}

				int top = Mth::Max(surface, stoneTop);
				if (top > topMap[x + z * WIDTH])
					topMap[x + z * WIDTH] = top;
			}
		}
	}

	for (int i = 0; i < WIDTH * LENGTH; i++)
		heightMap[i] = topMap[i];
}

void IndevLevelSource::_carveCaves()
{
	if (m_type == INDEV_TYPE_FLAT)
		return;

	int count = WIDTH * LENGTH * 64 / 256 / 64 << 1;

	for (int i = 0; i < count; i++)
	{
		float x = m_random.nextFloat() * WIDTH;
		float y = m_random.nextFloat() * DEPTH;
		float z = m_random.nextFloat() * LENGTH;
		int length = int((m_random.nextFloat() + m_random.nextFloat()) * 200.0f);

		float dir1 = m_random.nextFloat() * float(M_PI) * 2.0f;
		float dir1Change = 0.0f;
		float dir2 = m_random.nextFloat() * float(M_PI) * 2.0f;
		float dir2Change = 0.0f;
		float radiusBase = m_random.nextFloat() * m_random.nextFloat();

		for (int l = 0; l < length; l++)
		{
			x += Mth::sin(dir1) * Mth::cos(dir2);
			z += Mth::cos(dir1) * Mth::cos(dir2);
			y += Mth::sin(dir2);

			dir1 += dir1Change * 0.2f;
			dir1Change = dir1Change * 0.9f + (m_random.nextFloat() - m_random.nextFloat());
			dir2 = (dir2 + dir2Change * 0.5f) * 0.5f;
			dir2Change = dir2Change * 0.75f + (m_random.nextFloat() - m_random.nextFloat());

			if (m_random.nextFloat() < 0.25f)
				continue;

			float cx = x + (m_random.nextFloat() * 4.0f - 2.0f) * 0.2f;
			float cy = y + (m_random.nextFloat() * 4.0f - 2.0f) * 0.2f;
			float cz = z + (m_random.nextFloat() * 4.0f - 2.0f) * 0.2f;

			float r = (float(DEPTH) - cy) / float(DEPTH);
			r = 1.2f + (r * 3.5f + 1.0f) * radiusBase;
			r *= Mth::sin(float(l) * float(M_PI) / float(length));

			for (int xx = int(cx - r); xx <= int(cx + r); xx++)
			{
				for (int yy = int(cy - r); yy <= int(cy + r); yy++)
				{
					for (int zz = int(cz - r); zz <= int(cz + r); zz++)
					{
						float ddx = float(xx) - cx;
						float ddy = float(yy) - cy;
						float ddz = float(zz) - cz;
						if (ddx * ddx + ddy * ddy * 2.0f + ddz * ddz < r * r && yy >= 1 && _get(xx, yy, zz) == TILE_STONE)
							_set(xx, yy, zz, TILE_AIR);
					}
				}
			}
		}
	}
}

void IndevLevelSource::_populateOre(TileID ore, int abundance, int maxY)
{
	int count = WIDTH * LENGTH * DEPTH / 256 / 64 * abundance / 100;

	for (int i = 0; i < count; i++)
	{
		float x = m_random.nextFloat() * WIDTH;
		float y = m_random.nextFloat() * DEPTH;
		float z = m_random.nextFloat() * LENGTH;
		if (y > maxY)
			continue;

		int length = int((m_random.nextFloat() + m_random.nextFloat()) * 75.0f * abundance / 100.0f);

		float dir1 = m_random.nextFloat() * float(M_PI) * 2.0f;
		float dir1Change = 0.0f;
		float dir2 = m_random.nextFloat() * float(M_PI) * 2.0f;
		float dir2Change = 0.0f;

		for (int l = 0; l < length; l++)
		{
			x += Mth::sin(dir1) * Mth::cos(dir2);
			z += Mth::cos(dir1) * Mth::cos(dir2);
			y += Mth::sin(dir2);

			dir1 += dir1Change * 0.2f;
			dir1Change = dir1Change * 0.9f + (m_random.nextFloat() - m_random.nextFloat());
			dir2 = (dir2 + dir2Change * 0.5f) * 0.5f;
			dir2Change = dir2Change * 0.9f + (m_random.nextFloat() - m_random.nextFloat());

			float r = Mth::sin(float(l) * float(M_PI) / float(length)) * float(abundance) / 100.0f + 1.0f;

			for (int xx = int(x - r); xx <= int(x + r); xx++)
			{
				for (int yy = int(y - r); yy <= int(y + r); yy++)
				{
					for (int zz = int(z - r); zz <= int(z + r); zz++)
					{
						float ddx = float(xx) - x;
						float ddy = float(yy) - y;
						float ddz = float(zz) - z;
						if (ddx * ddx + ddy * ddy * 2.0f + ddz * ddz < r * r && yy <= maxY && _get(xx, yy, zz) == TILE_STONE)
							_set(xx, yy, zz, ore);
					}
				}
			}
		}
	}
}

// Spreads sideways and downwards only, like the Classic/Indev flood fill.
void IndevLevelSource::_floodFill(int x, int y, int z, TileID liquid)
{
	if (_get(x, y, z) != TILE_AIR)
		return;

	std::vector<int> stack;
	stack.push_back(_index(x, y, z));
	m_pMap[_index(x, y, z)] = liquid;

	while (!stack.empty())
	{
		int idx = stack.back();
		stack.pop_back();

		int cx = idx >> 15;
		int cz = (idx >> 7) & 0xFF;
		int cy = idx & 0x7F;

		const int offsets[5][3] = { { 1, 0, 0 }, { -1, 0, 0 }, { 0, 0, 1 }, { 0, 0, -1 }, { 0, -1, 0 } };
		for (int i = 0; i < 5; i++)
		{
			int nx = cx + offsets[i][0];
			int ny = cy + offsets[i][1];
			int nz = cz + offsets[i][2];
			if (!_inBounds(nx, ny, nz) || ny < 1)
				continue;

			int nidx = _index(nx, ny, nz);
			if (m_pMap[nidx] != TILE_AIR)
				continue;

			m_pMap[nidx] = liquid;
			stack.push_back(nidx);
		}
	}
}

void IndevLevelSource::_flood()
{
	if (m_type == INDEV_TYPE_FLOATING || m_type == INDEV_TYPE_FLAT)
		return;

	// the ocean surrounding the map
	if (m_type == INDEV_TYPE_ISLAND)
	{
		int y = m_waterLevel - 1;
		for (int x = 0; x < WIDTH; x++)
		{
			_floodFill(x, y, 0, m_liquid);
			_floodFill(x, y, LENGTH - 1, m_liquid);
		}
		for (int z = 0; z < LENGTH; z++)
		{
			_floodFill(0, y, z, m_liquid);
			_floodFill(WIDTH - 1, y, z, m_liquid);
		}
	}

	// lakes & underground water
	int waterCount = WIDTH * LENGTH / 8000;
	if (m_type == INDEV_TYPE_INLAND)
		waterCount *= 4;

	for (int i = 0, tries = 0; i < waterCount && tries < waterCount * 50; tries++)
	{
		int x = m_random.nextInt(WIDTH);
		int z = m_random.nextInt(LENGTH);
		int y = m_waterLevel - 1 - m_random.nextInt(2);

		// inland maps keep looking for a dip in the ground to make a lake in
		if (m_type == INDEV_TYPE_INLAND && _get(x, y, z) != TILE_AIR)
			continue;

		_floodFill(x, y, z, m_liquid);
		i++;
	}

	// underground lava
	int lavaCount = WIDTH * LENGTH * 64 / 20000;
	for (int i = 0; i < lavaCount; i++)
	{
		int x = m_random.nextInt(WIDTH);
		int y = int(m_random.nextFloat() * m_random.nextFloat() * float(m_waterLevel - 8));
		int z = m_random.nextInt(LENGTH);
		_floodFill(x, y, z, TILE_LAVA_CALM);
	}
}

void IndevLevelSource::_grow(std::vector<int>& heightMap)
{
	PerlinNoise sandNoise(&m_random, 8);
	PerlinNoise gravelNoise(&m_random, 8);

	const bool hell = m_theme == INDEV_THEME_HELL;
	const float sandThreshold = (m_theme == INDEV_THEME_PARADISE) ? 0.0f : 8.0f;

	for (int x = 0; x < WIDTH; x++)
	{
		for (int z = 0; z < LENGTH; z++)
		{
			bool sand = sandNoise.getValue(float(x), float(z)) > sandThreshold;
			bool gravel = gravelNoise.getValue(float(x), float(z)) > 12.0f;

			int y = heightMap[x + z * WIDTH];
			if (y < 1)
				continue;

			TileID above = _get(x, y + 1, z);

			if ((above == TILE_WATER_CALM || above == TILE_LAVA_CALM) && gravel)
				_set(x, y, z, TILE_GRAVEL);

			if (above == TILE_AIR)
			{
				if (y <= m_waterLevel && sand && m_type != INDEV_TYPE_FLOATING && m_type != INDEV_TYPE_FLAT)
					_set(x, y, z, TILE_SAND);
				else if (_get(x, y, z) == TILE_DIRT && !hell)
					_set(x, y, z, TILE_GRASS);
			}
		}
	}

	// Flat maps: a grass plain right above the water line
	if (m_type == INDEV_TYPE_FLAT)
	{
		for (int x = 0; x < WIDTH; x++)
			for (int z = 0; z < LENGTH; z++)
				if (_get(x, m_waterLevel, z) == TILE_DIRT && _get(x, m_waterLevel + 1, z) == TILE_AIR)
					_set(x, m_waterLevel, z, hell ? TILE_DIRT : TILE_GRASS);
	}
}

bool IndevLevelSource::_growTree(int x, int y, int z)
{
	int height = m_random.nextInt(3) + 4;

	if (y < 1 || y + height + 1 >= DEPTH)
		return false;

	TileID below = _get(x, y - 1, z);
	if (below != TILE_GRASS && below != TILE_DIRT)
		return false;

	// check space
	for (int yy = y; yy <= y + 1 + height; yy++)
	{
		int r = 1;
		if (yy == y) r = 0;
		if (yy >= y + 1 + height - 2) r = 2;

		for (int xx = x - r; xx <= x + r; xx++)
		{
			for (int zz = z - r; zz <= z + r; zz++)
			{
				if (!_inBounds(xx, yy, zz))
					return false;
				TileID t = _get(xx, yy, zz);
				if (t != TILE_AIR && t != TILE_LEAVES)
					return false;
			}
		}
	}

	_set(x, y - 1, z, TILE_DIRT);

	for (int yy = y - 3 + height; yy <= y + height; yy++)
	{
		int dy = yy - (y + height);
		int r = 1 - dy / 2;
		for (int xx = x - r; xx <= x + r; xx++)
		{
			for (int zz = z - r; zz <= z + r; zz++)
			{
				int dx = xx - x, dz = zz - z;
				if (Mth::abs(dx) == r && Mth::abs(dz) == r && (m_random.nextInt(2) == 0 || dy == 0))
					continue;
				if (_get(xx, yy, zz) == TILE_AIR)
					_set(xx, yy, zz, TILE_LEAVES);
			}
		}
	}

	for (int yy = 0; yy < height; yy++)
		_set(x, y + yy, z, TILE_TREE_TRUNK);

	return true;
}

void IndevLevelSource::_plant(std::vector<int>& heightMap)
{
	const bool hell = m_theme == INDEV_THEME_HELL;

	// flowers
	if (!hell)
	{
		int flowerCount = WIDTH * LENGTH / 3000;
		if (m_theme == INDEV_THEME_PARADISE)
			flowerCount *= 6;

		for (int i = 0; i < flowerCount; i++)
		{
			TileID flower = m_random.nextInt(2) == 0 ? TILE_FLOWER : TILE_ROSE;
			int x = m_random.nextInt(WIDTH);
			int z = m_random.nextInt(LENGTH);

			for (int j = 0; j < 10; j++)
			{
				int fx = x, fz = z;
				for (int k = 0; k < 5; k++)
				{
					fx += m_random.nextInt(6) - m_random.nextInt(6);
					fz += m_random.nextInt(6) - m_random.nextInt(6);
					if (fx < 0 || fz < 0 || fx >= WIDTH || fz >= LENGTH)
						continue;

					int fy = heightMap[fx + fz * WIDTH] + 1;
					if (_get(fx, fy, fz) == TILE_AIR && _get(fx, fy - 1, fz) == TILE_GRASS)
						_set(fx, fy, fz, flower);
				}
			}
		}
	}

	// mushrooms, in caves (and on the surface of hell maps)
	int mushroomCount = WIDTH * LENGTH * 64 / 2000;
	for (int i = 0; i < mushroomCount; i++)
	{
		TileID mushroom = m_random.nextInt(2) == 0 ? TILE_MUSHROOM_1 : TILE_MUSHROOM_2;
		int x = m_random.nextInt(WIDTH);
		int z = m_random.nextInt(LENGTH);
		int y = m_random.nextInt(DEPTH);

		for (int j = 0; j < 20; j++)
		{
			int mx = x, my = y, mz = z;
			for (int k = 0; k < 5; k++)
			{
				mx += m_random.nextInt(6) - m_random.nextInt(6);
				my += m_random.nextInt(2) - m_random.nextInt(2);
				mz += m_random.nextInt(6) - m_random.nextInt(6);
				if (mx < 0 || mz < 0 || mx >= WIDTH || mz >= LENGTH || my < 1 || my >= DEPTH - 1)
					continue;

				bool underground = my < heightMap[mx + mz * WIDTH];
				if (!underground && (!hell || m_random.nextInt(4) != 0))
					continue;

				TileID below = _get(mx, my - 1, mz);
				if (_get(mx, my, mz) == TILE_AIR && (below == TILE_STONE || below == TILE_DIRT || below == TILE_GRASS))
					_set(mx, my, mz, mushroom);
			}
		}
	}

	// trees
	int treeCount = WIDTH * LENGTH / 4000;
	switch (m_theme)
	{
	case INDEV_THEME_WOODS: treeCount *= 6; break;
	case INDEV_THEME_HELL:  treeCount /= 4; break;
	default: break;
	}

	for (int i = 0; i < treeCount; i++)
	{
		int x = m_random.nextInt(WIDTH);
		int z = m_random.nextInt(LENGTH);

		for (int j = 0; j < 20; j++)
		{
			int tx = x, tz = z;
			for (int k = 0; k < 20; k++)
			{
				tx += m_random.nextInt(6) - m_random.nextInt(6);
				tz += m_random.nextInt(6) - m_random.nextInt(6);
				if (tx < 0 || tz < 0 || tx >= WIDTH || tz >= LENGTH)
					continue;

				int ty = heightMap[tx + tz * WIDTH] + 1;
				if (m_random.nextFloat() < 0.25f)
					_growTree(tx, ty, tz);
			}
		}
	}
}

LevelChunk* IndevLevelSource::getChunk(const ChunkPos& pos)
{
	int hash = (pos.x & 0xFFFF) | ((pos.z & 0xFFFF) << 16);
	std::map<int, LevelChunk*>::iterator iter = m_chunks.find(hash);
	if (iter != m_chunks.end())
		return iter->second;

	TileID* pData = new TileID[16 * 16 * DEPTH];

	if (pos.x < 0 || pos.z < 0 || pos.x >= C_MAX_CHUNKS_X || pos.z >= C_MAX_CHUNKS_Z)
	{
		memset(pData, 0, sizeof(TileID) * 16 * 16 * DEPTH);
	}
	else
	{
		_generate();

		for (int lx = 0; lx < 16; lx++)
		{
			for (int lz = 0; lz < 16; lz++)
			{
				int gx = pos.x * 16 + lx;
				int gz = pos.z * 16 + lz;
				memcpy(&pData[(lx << 11) | (lz << 7)], &m_pMap[_index(gx, 0, gz)], sizeof(TileID) * DEPTH);
			}
		}
	}

	LevelChunk* pChunk = new LevelChunk(m_pLevel, pData, pos);
	m_chunks.insert(std::pair<int, LevelChunk*>(hash, pChunk));
	pChunk->recalcHeightmap();

	return pChunk;
}

LevelChunk* IndevLevelSource::getChunkDontCreate(const ChunkPos& pos)
{
	return getChunk(pos);
}

LevelChunk* IndevLevelSource::create(const ChunkPos& pos)
{
	return getChunk(pos);
}

void IndevLevelSource::postProcess(ChunkSource*, const ChunkPos& pos)
{
	// Everything has already been placed while generating the map.
}

int IndevLevelSource::tick()
{
	return 0;
}

bool IndevLevelSource::shouldSave()
{
	return true;
}

bool IndevLevelSource::hasChunk(const ChunkPos& pos)
{
	return true;
}

std::string IndevLevelSource::gatherStats()
{
	return "IndevLevelSource";
}
