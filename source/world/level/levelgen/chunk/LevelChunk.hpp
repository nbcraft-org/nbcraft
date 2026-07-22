/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include <cstring>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <atomic>
#include <mutex>
#include "world/level/levelgen/chunk/ChunkPos.hpp"
#include "world/level/levelgen/chunk/ChunkTilePos.hpp"
#include "world/level/TileTickingQueue.hpp"
#include "common/Random.hpp"
#include "common/Utils.hpp"
#include "client/renderer/LightLayer.hpp"
#include "world/level/levelgen/chunk/ChunkConstants.hpp"
#include "world/tile/Tile.hpp"

class Level;
class Dimension;
class AABB;
class Entity;
class TileEntity;
class ChunkSource;
class Biome;

struct BiomeChunkData
{
	uint8_t m_biomeId;
	uint8_t m_grassRed;
	uint8_t m_grassGreen;
	uint8_t m_grassBlue;
};

enum ChunkState
{
	CS_UNLOADED,
	CS_GENERATING,
	CS_GENERATED,
	CS_POST_PROCESSING,
	CS_POST_PROCESSED,
	CS_CHECKING_FOR_REPLACEMENT_DATA,
	CS_NEEDS_LIGHTING,
	CS_LIGHTING,
	CS_LOADED
};


// TODO: CHECK THIS
// TODO: MOVE THIS TO CHUNKTILEPOS.CPP
template <>
struct std::hash<ChunkTilePos>
{
	size_t operator()(const ChunkTilePos& k) const
	{
		return ((std::hash<uint8_t>()(k.y)
			^ (std::hash<uint8_t>()(k.z) << 1)) >> 1)
			^ (std::hash<uint8_t>()(k.x) << 1);
	}
};

class LevelChunk
{
public:
	enum Finalization
	{
		CF_NEEDS_INSTA_TICKING,
		CF_NEEDS_POPULATION,
		CF_DONE
	};

public:
	// Previously called "DataLayer"
	struct NibbleTileArray
	{
		inline NibbleTileArray()
		{
			array = new uint8_t[getSize()];
            memset(array, 0, getSize());
		}

		inline ~NibbleTileArray()
		{
			delete[] array;
		}

		inline uint8_t get(int index) const
		{
			uint8_t byte = array[index >> 1];

			if ((index & 1) == 0)
			{
				// get low bits
				return byte & 0xF;
			}
			else
			{
				// get high bits
				return (byte >> 4) & 0xF;
			}
		}

		inline uint8_t get(const ChunkTilePos& pos) const
		{
			return get(pos.index());
		}

		inline bool set(int index, uint8_t value)
		{
			assert(value <= 15);

			int idx = index >> 1;
			uint8_t byte = array[idx];

			if ((index & 1) == 0)
			{
				// low bits
				if ((byte & 0xF) != value)
				{
					value &= 0xF;
					array[idx] = (byte & 0xF0) | value;
					return true;
				}
			}
			else
			{
				// high bits
				if ((byte >> 4) != value)
				{
					value &= 0xF;
					array[idx] = (value << 4) | (byte & 0x0F);
					return true;
				}
			}

			return false;
		}

		inline bool set(const ChunkTilePos& pos, uint8_t value)
		{
			return set(pos.index(), value);
		}

		inline size_t getSize() const { return ChunkConstants::TILE_COUNT / 2; }

		uint8_t* array;
	};

public:
	LevelChunk(Level& level, Dimension& dimension, const ChunkPos& pos, bool readOnly = false);
	~LevelChunk();

public:
	bool isAt(const ChunkPos& pos) const
	{
		return m_pos == pos;
	}
	bool isAt(int x, int z) const
	{
		return isAt(ChunkPos(x, z));
	}
	uint8_t getHeightmap(const ChunkTilePos& pos) const;
	void recalcHeightmap();
	int getBrightness(const LightLayer& lightLayer, const ChunkTilePos& pos) const;
	void setBrightness(const LightLayer& lightLayer, const ChunkTilePos& pos, Brightness_t brightness);
	Brightness_t getRawBrightness(const ChunkTilePos& pos, Brightness_t skyDampen) const;
	bool isSkyLit(const ChunkTilePos& pos) const;
	void recalcBlockLights();
	size_t countEntities() const
	{
		return m_entities.size();
	}
	void addEntity(std::unique_ptr<Entity> entity);
	bool removeEntity(Entity& entity);
	void moveEntity(std::unique_ptr<Entity>& entity);
	void getEntities(Entity* blacklistedEntity, const AABB& aabb, std::vector<Entity*>& output) const;
	void getEntities(const EntityType& type, const AABB& aabb, std::vector<Entity*>& output) const;
	const ChunkPos& getPos() const
	{
		return m_pos;
	}
	const TilePos& getMinPos() const
	{
		return m_posMin;
	}
	const TilePos& getMaxPos() const
	{
		return m_posMax;
	}
	TileID getTile(const ChunkTilePos& pos) const
	{
		return m_tiles[pos.index()];
	}
	TileData getData(const ChunkTilePos& pos) const
	{
		return m_tileData.get(pos);
	}
	FullTile getTileAndData(const ChunkTilePos& pos) const
	{
		return FullTile(getTile(pos), getData(pos));
	}
	bool setData(const ChunkTilePos& pos, TileData data);
	FullTile setTileAndData(const ChunkTilePos& pos, FullTile tile, TileSource* issuingSource); // Returns the old tile and data at the position
	Biome* getBiome(const ChunkTilePos& pos);
	uint32_t getGrassColor(const ChunkTilePos& pos);

	void tick(Player* player, Tick_t tick);
	void tickBlocks(Player* player);

	bool isReadOnly() const
	{
		return m_readOnly;
	}

	TileTickingQueue& getTileTickingQueue()
	{
		return m_tileTickingQueue;
	}

	TileID* getTiles()
	{
		return m_tiles;
	}

	NibbleTileArray& getTileData()
	{
		return m_tileData;
	}

	void setGenerator(ChunkSource& generator)
	{
		m_generator = &generator;
	}

	ChunkSource* getGenerator()
	{
		return m_generator;
	}

	// TODO: determine if this should be const
	NibbleTileArray& getLight(const LightLayer& lightLayer);

	ChunkState getState() const
	{
		return m_state.load(std::memory_order_acquire);
	}

	void setFinalized(Finalization);
	void changeState(ChunkState, ChunkState);
	bool tryChangeState(ChunkState, ChunkState);

	void deferLightEmitter(const TilePos& pos);

	void setSaved();

	Random getRandom(int32_t l);

protected:
	void _resetDirtyCounter(int& dirtyCounter, int to);

	void _placeCallbacks(const ChunkTilePos&, TileID, TileID, TileSource*);
	void _removeCallbacks(const ChunkTilePos&, TileID, TileID, TileSource*);

	void _lightGap(TileSource& source, const TilePos& pos);
	void _lightGaps(TileSource& source, const ChunkTilePos& pos);

protected:
	static void AssertPos(const ChunkTilePos& pos)
	{
		assert(pos.x >= 0 && pos.y >= 0 && pos.z >= 0 && pos.x < ChunkConstants::XZ_SIZE && pos.z < ChunkConstants::XZ_SIZE && pos.y < ChunkConstants::Y_SIZE);
	}

protected:
	Level& m_level;
	Dimension& m_dimension;
	TilePos m_posMin;
	TilePos m_posMax;
	ChunkPos m_pos;
	bool m_readOnly;
	ChunkSource* m_generator;
	std::mutex m_deferredLightMutex;
	std::vector<TilePos> m_deferredLightEmitterPositions;
	std::string m_pendingEntitiesSerialized;
	std::atomic<ChunkState> m_state;
	Tick_t m_currentTick;
	TileTickingQueue m_tileTickingQueue;
	TileID m_tiles[ChunkConstants::TILE_COUNT];
	NibbleTileArray m_tileData;
	NibbleTileArray m_lightSky;
	NibbleTileArray m_lightBlock;
	BiomeChunkData m_biomes[ChunkConstants::TILE_COUNT_PER_ROW];
	uint8_t m_heightMap[ChunkConstants::TILE_COUNT_PER_ROW];
	//std::unordered_map<uint8_t, BiomeChunkState> m_biomeStates;
	Finalization m_finalization;
	int m_biomeDirtyTicks;
	int m_terrainDirtyTicks;
	int m_tileEntityDirtyTicks;
	int m_entityDirtyTicks;
	int16_t m_unk85876[ChunkConstants::TILE_COUNT_PER_ROW];
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::unordered_map<ChunkTilePos, std::unique_ptr<TileEntity>> m_tileEntities;
};
