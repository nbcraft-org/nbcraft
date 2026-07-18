/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "TileSource.hpp"

class Region : public TileSource
{
public:
	Region(const Level* level, const TilePos& min, const TilePos& max);
	~Region();

public:
	TileID getTile(const TilePos& pos) const override;
	Brightness_t getRawBrightness(const TilePos& pos, bool b) const override;
	Brightness_t getRawBrightness(const TilePos& pos) const override;
	float getBrightness(const TilePos& pos) const override;
	TileData getData(const TilePos& pos) const override;
	Material* getMaterial(const TilePos& pos) const override;
	bool isSolidBlockingTile(const TilePos& pos) const override;
	BiomeSource* getBiomeSource() const override;
	TileEntity* getTileEntity(const TilePos& pos) const override;

	// inlined in the original, but I doubt they'd actually copy paste this logic
	LevelChunk* getChunk(const ChunkPos& pos) const override
	{
		int indexX = pos.x - field_4.x;
		int indexZ = pos.z - field_4.z;

		if (indexX < 0 || indexZ < 0 || indexX >= field_14.x || indexZ >= field_14.z)
			return nullptr;

		return field_C[indexZ * field_14.x + indexX];
	}
	LevelChunk* getChunkAt(const TilePos& pos) const override { return getChunk(pos); }

	Level& getLevel() const override { throw std::bad_cast(); }
	const Level& getLevelConst() const override { throw std::bad_cast(); }
	Dimension& getDimension() const override { throw std::bad_cast(); }
	const Dimension& getDimensionConst() const override { throw std::bad_cast(); }
	ChunkSource& getChunkSource() const override { throw std::bad_cast(); }
	void setTickingQueue(TileTickingQueue& queue) override { throw std::bad_cast(); }
	TileTickingQueue* getTickQueue(const TilePos& pos) const override { throw std::bad_cast(); }
	bool hasChunk(const ChunkPos& pos) const override { throw std::bad_cast(); } // from Level
	bool hasChunkAt(const TilePos& pos) const override { throw std::bad_cast(); }
	bool hasChunksAt(const TilePos& min, const TilePos& max) const override { throw std::bad_cast(); }
	bool hasChunksAt(const TilePos& pos, int r) const override { throw std::bad_cast(); }
	Brightness_t getSkyDarken() const override { throw std::bad_cast(); }
	int getSignal(const TilePos& pos, Facing::Name face) const override { throw std::bad_cast(); }
	int getDirectSignal(const TilePos& pos, Facing::Name face) const override { throw std::bad_cast(); }
	bool hasDirectSignal(const TilePos& pos) const override { throw std::bad_cast(); }
	bool hasNeighborSignal(const TilePos& pos) const override { throw std::bad_cast(); }
	void fireTilesDirty(const TilePos&, const TilePos&) override { throw std::bad_cast(); } // unk type
	DimensionId getDimensionId() const override { throw std::bad_cast(); } // unk type
	TileID getTopSolidBlock(const TilePos&, bool b = false) const override { throw std::bad_cast(); } // @TODO: check the default value on the bool
	bool canSeeSky(const TilePos&) const override { throw std::bad_cast(); }
	bool isSkyLit(const TilePos&) const override { throw std::bad_cast(); }
	uint8_t getHeightmap(const TilePos&) const override { throw std::bad_cast(); }
	const TileEntity::Vector& getAllTileEntities() const override { throw std::bad_cast(); }
	void setTileEntity(const TilePos& pos, TileEntity* tileEntity) override { throw std::bad_cast(); }
	void removeTileEntity(const TilePos& pos) override { throw std::bad_cast(); }
	bool containsMaterial(const AABB&, const Material*) const override { throw std::bad_cast(); }
	void tileEvent(const TileEvent& event) override { throw std::bad_cast(); } // unk type
	bool containsAnyLiquid(const AABB&) const override { throw std::bad_cast(); }
	bool containsFireTile(const AABB&) const override { throw std::bad_cast(); }
	void updateNeighborsAt(const TilePos&, TileID) override { throw std::bad_cast(); } // unk type
	bool isEmptyTile(const TilePos&) const override { throw std::bad_cast(); }
	Brightness_t getBrightness(const LightLayer& lightLayer, const TilePos& pos) const override { throw std::bad_cast(); }
	void updateLightIfOtherThan(const LightLayer&, const TilePos&, Brightness_t) override { throw std::bad_cast(); } // unk type
	bool containsLiquid(const AABB&, const Material*) const override { throw std::bad_cast(); }
	HitResult clip(const Vec3& A, const Vec3& B, bool liquid = false, bool solidOnly = false) const override { throw std::bad_cast(); }
	float getSeenPercent(const Vec3&, const AABB&) const override { throw std::bad_cast(); }
	Biome& getBiome(const TilePos&) const override { throw std::bad_cast(); }
	bool setTileAndData(const TilePos&, const FullTile&, TileChange updateFlags = TileChange::UPDATE_NONE) override { throw std::bad_cast(); }
	bool setTile(const TilePos&, TileID, TileChange updateFlags = TileChange::UPDATE_NONE) override { throw std::bad_cast(); }
	bool setTileAndDataNoUpdate(const TilePos&, const FullTile&) override { throw std::bad_cast(); }
	bool setTileNoUpdate(const TilePos&, TileID) override { throw std::bad_cast(); }
	void setBrightness(const LightLayer&, const TilePos&, Brightness_t) override { throw std::bad_cast(); }
	std::vector<AABB>& fetchAABBs(const AABB&, bool b = true) override { throw std::bad_cast(); } // unk type
	std::vector<Entity*> getEntities(const EntityType&, const AABB&, Entity*) const override { throw std::bad_cast(); } // unk type
	std::vector<Entity*> getEntities(Entity* except, const AABB& bb) const override { throw std::bad_cast(); } // from Level
	bool isUnobstructedByEntities(const AABB&, Entity*) const override { throw std::bad_cast(); }
	bool mayInteract(Entity* entity, const TilePos& pos) const override { throw std::bad_cast(); }
	bool mayPlace(TileID tileId, const TilePos& pos, bool ignoreEntities = false) const override { throw std::bad_cast(); }
	bool mayPlace(TileID tileId, const TilePos& pos, Facing::Name face, Entity& placer, bool ignoreEntities = false, Entity* ignoreEntity = nullptr) const override { throw std::bad_cast(); }
	void addListener(TileSourceListener& listener) override { throw std::bad_cast(); }
	void removeListener(TileSourceListener& listener) override { throw std::bad_cast(); }

private:
	ChunkPos field_4;
	// accesses to the array are performed as follows:
	// (x = 0..m_noEntityRenderFrames, z = 0..m_totalEntities)
	// z * m_noEntityRenderFrames + x
	LevelChunk** field_C;
	const Level* m_pLevel;
	ChunkPos field_14;
};

