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
#include "common/Random.hpp"
#include "common/Utils.hpp"
#include "client/renderer/LightLayer.hpp"
#include "world/level/levelgen/chunk/ChunkPos.hpp"
#include "world/level/levelgen/chunk/ChunkTilePos.hpp"
#include "world/level/levelgen/chunk/DataLayer.hpp"

class Level;
class AABB;
class Entity;
class EntityType;
class TileEntity;

class LevelChunk
{
public:
	struct NibbleTileArray
	{
		NibbleTileArray()
		{
			array = new uint8_t[getSize()];
            memset(array, 0, getSize());
		}

		~NibbleTileArray()
		{
			delete[] array;
		}

		uint8_t get(const ChunkTilePos& pos) const
		{
			int index = pos.index();
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

		bool set(const ChunkTilePos& pos, uint8_t value)
		{
			assert(value <= 15);

			int index = pos.index();
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
					uint8_t val = (value << 4) || (byte & 0x0F);
					array[idx] = val; // tf are we actually writing??
					return true;
				}
			}

			return false;
		}

		size_t getSize() const { return ChunkConstants::TILE_COUNT / 2; }

		uint8_t* array;
	};

private:
	void _init();
protected:
	LevelChunk() { _init(); }
public:
	LevelChunk(Level*, const ChunkPos& pos);
	LevelChunk(Level*, TileID* pBlockData, const ChunkPos& pos);
	virtual ~LevelChunk();

	void init();

	void lightGap(const TilePos& pos, uint8_t heightMap);
	void lightGaps(const ChunkTilePos& pos);
	void deleteBlockData();
	void clearUpdateMap();

	virtual bool isAt(const ChunkPos& pos);
	virtual int getHeightmap(const ChunkTilePos& pos);
	virtual void recalcHeightmap();
	virtual void recalcHeightmapOnly();
	virtual int getBrightness(const LightLayer& ll, const ChunkTilePos& pos);
	virtual void setBrightness(const LightLayer& ll, const ChunkTilePos& pos, int brightness);
	virtual int getRawBrightness(const ChunkTilePos& pos, int skySubtract);
	virtual void addEntity(Entity*);
	virtual void removeEntity(Entity*);
	virtual void removeEntity(Entity*, int vec);
	virtual void updateEntity(Entity* pEnt);
	virtual bool isSkyLit(const ChunkTilePos& pos);
	virtual void lightLava();
	virtual void recalcBlockLights();
	virtual void skyBrightnessChanged();
	virtual void load();
	virtual void unload();
	virtual bool shouldSave(bool b);
	virtual void markUnsaved();
	virtual int  countEntities();
	virtual void getEntities(Entity* pEntExclude, const AABB&, std::vector<Entity*>& out);
	virtual void getEntities(const EntityType& type, const AABB& aabb, std::vector<Entity*>& output) const;
	virtual void getEntities(const EntityType& type, const AABB& aabb, Entity* pEntExclude, std::vector<Entity*>& output) const;
	virtual TileID getTile(const ChunkTilePos& pos);
	virtual bool setTile(const ChunkTilePos& pos, TileID tile);
	virtual bool setTileAndData(const ChunkTilePos& pos, TileID tile, TileData data);
	virtual TileData getData(const ChunkTilePos& pos);
	virtual bool setData(const ChunkTilePos& pos, TileData data);
	virtual void setBlocks(uint8_t* pData, int y);
	virtual int  getBlocksAndData(uint8_t* pData, int, int, int, int, int, int, int);
	virtual int  setBlocksAndData(uint8_t* pData, int, int, int, int, int, int, int);
	virtual TileEntity* getTileEntity(const ChunkTilePos& pos);
	virtual void addTileEntity(TileEntity* tileEntity);
	virtual void setTileEntity(const ChunkTilePos& pos, TileEntity* tileEntity);
	virtual void removeTileEntity(const ChunkTilePos& pos);
	virtual Random getRandom(int32_t l);
	virtual void recalcHeight(const ChunkTilePos& pos);
	virtual bool isEmpty();
	//...

	TileID* getTiles() { return m_pBlockData; }
	NibbleTileArray& getTileData() { return m_tileData; }
	const ChunkPos& getPos() const { return m_chunkPos; }

public:
	static bool touchedSky;

public:
	int field_4;
	bool m_bLoaded;
	Level* m_pLevel;
	NibbleTileArray m_tileData;
	NibbleTileArray m_lightSky;
	NibbleTileArray m_lightBlk;
	uint8_t m_heightMap[256];
	uint8_t m_updateMap[256];
	int field_228;
	ChunkPos m_chunkPos;
	uint8_t field_234;
	bool m_bUnsaved;
	uint8_t field_236;
	uint8_t field_237;
	int field_238;
	int field_23C;
	TileID* m_pBlockData;
	std::vector<Entity*> m_entities[128 / 16];
	std::map<ChunkTilePos, TileEntity*> m_tileEntities;
};
