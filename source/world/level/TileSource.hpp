#pragma once
#include <memory>
#include <thread>
#include "client/renderer/LightLayer.hpp"
#include "world/entity/EntityType.hpp"
#include "world/entity/MobSpawnerData.hpp"
#include "world/level/levelgen/biome/Biome.hpp"
#include "world/level/levelgen/chunk/ChunkPos.hpp"
#include "world/level/Brightness.hpp"
#include "world/level/TileChange.hpp"
#include "world/tile/Tile.hpp"

class TileTickingQueue;
class AABB;
class LevelChunk;
class TileEntity;
class TileSourceListener; // TODO
class TileTickingQueue;
class Dimension;
class ChunkSource;
struct Bounds;

class TileSource
{
protected:
	typedef std::vector<TileSourceListener*> Listeners;

public:
	TileSource(Level& level, Dimension& dimension, ChunkSource& source, bool publicSource, bool allowUnpopulatedChunks);
	virtual ~TileSource();

protected:
	void _neighborChanged(const TilePos&, const TilePos&, TileID);
	void _tileChanged(const TilePos& pos, FullTile oldTile, FullTile newTile, TileChange updateFlags);

public:
	Level& getLevel() const;
	const Level& getLevelConst() const;
	Dimension& getDimension() const;
	const Dimension& getDimensionConst() const;
	ChunkSource& getChunkSource() const;
	void setTickingQueue(TileTickingQueue& queue);
	TileTickingQueue* getTickQueue(const TilePos& pos);
	LevelChunk* getChunk(const ChunkPos& pos);
	LevelChunk* getWritableChunk(const ChunkPos& pos);
	bool shouldFireEvents(LevelChunk& chunk) const;
	bool hasChunk(const ChunkPos& pos); // from Level
	bool hasChunkAt(const TilePos& pos);
	bool hasChunksAt(const Bounds& bounds);
	bool hasChunksAt(const AABB& aabb);
	bool hasChunksAt(const TilePos& min, const TilePos& max);
	bool hasChunksAt(const TilePos& pos, int r);
	Brightness_t getSkyDarken() const;
	const std::vector<MobSpawnerData>& getMobsAt(EntityType entityType, const TilePos& pos);
	int getSignal(const TilePos& pos, Facing::Name face);
	int getDirectSignal(const TilePos& pos, Facing::Name face);
	bool hasDirectSignal(const TilePos& pos);
	bool hasNeighborSignal(const TilePos& pos);
	void runLightUpdates(const LightLayer&, const TilePos&, const TilePos&); // unk type
	bool isTopSolidBlocking(Tile*, uint8_t);
	void onChunkDiscarded(LevelChunk& chunk);
	void clearCachedLastChunk(); // unk type
	void fireAreaChanged(const TilePos&, const TilePos&); // unk type
	void fireTilesDirty(const TilePos&, const TilePos&); // unk type
	void fireTileChanged(const TilePos&, const FullTile&, const FullTile&, TileChange);
	void fireBrightnessChanged(const TilePos&); // unk type
	void fireTileEntityRemoved(std::unique_ptr<TileEntity>&); // unk type
	void fireTileEvent(int, int, int, int, int); // unk type
	LevelChunk* getChunkAt(const TilePos&);
	void hasTile(const TilePos&); // unk type
	void fireTileEntityChanged(TileEntity&); // unk type
	bool isOwnerThread() const;
	DimensionId getDimensionId() const;
	TileData setExtraData(const TilePos&, uint16_t); // unk type
	TileData getExtraData(const TilePos&); // unk type
	TileID getTopSolidBlock(const TilePos&, bool b = false); // @TODO: check the default value on the bool
	bool canSeeSky(const TilePos&) const;
	bool isSkyLit(const TilePos&) const;
	uint8_t getHeightmap(const TilePos&) const;
	void getHeightmapPos(const TilePos&) const; // unk type
	TileEntity* getTileEntity(const TilePos& pos) const;
	const std::vector<TileEntity*>& getAllTileEntities() const;
	void setTileEntity(const TilePos& pos, TileEntity* tileEntity);
	void removeTileEntity(const TilePos& pos);
	void getGrassColor(const TilePos&); // unk type
	void getTopRainTilePos(const TilePos&); // unk type
	void checkIsTopRainTilePos(const TilePos&); // unk type
	TileID getTile(const TilePos& pos);
	Tile* getTilePtr(const TilePos& pos);
	TileData getData(const TilePos& pos);
	bool isSolidBlockingTile(const TilePos&);
	bool isSolidRenderTile(const TilePos&);
	bool containsMaterial(const AABB&, const Material*) const;
	void tileEvent(const TileEvent& event); // unk type
	bool containsAnyLiquid(const AABB&) const;
	bool containsFireTile(const AABB&) const;
	Material* getMaterial(const TilePos&) const;
	void isWaterAt(const TilePos&) const; // unk type
	void isIceAt(const TilePos&) const; // unk type
	void updateNeighborsAt(const TilePos&, TileID); // unk type
	bool isEmptyTile(const TilePos&) const;
	void findNextTopSolidTileUnder(TilePos&); // unk type
	void containsAny(const TilePos&, const TilePos&); // unk type
	Brightness_t getRawBrightness(const TilePos& pos, bool b);
	Brightness_t getRawBrightness(const TilePos& pos) { return getRawBrightness(pos, true); }
	Brightness_t getBrightness(const LightLayer& lightLayer, const TilePos& pos);
	float getBrightness(const TilePos& pos);
	void updateLightIfOtherThan(const LightLayer&, const TilePos&, Brightness_t); // unk type
	bool containsLiquid(const AABB&, const Material*) const;
	FullTile getTileAndData(const TilePos&) const; // unk type
	bool isTopSolidBlocking(const TilePos&) const;
	HitResult clip(const Vec3& A, const Vec3& B, bool liquid = false, bool solidOnly = false) const;
	float getSeenPercent(const Vec3&, const AABB&) const;
	void getLightColor(const TilePos&, int) const; // unk type
	Biome& getBiome(const TilePos&) const;
	bool shouldFreeze(const TilePos&, bool);
	bool shouldFreezeIgnoreNeighbors(const TilePos&);
	bool shouldThaw(const TilePos&, bool);
	bool isSnowTemperature(const TilePos&);
	bool isHumidAt(const TilePos&);
	bool setTileAndData(const TilePos&, const FullTile&, TileChange updateFlags = TileChange::UPDATE_ALL);
	bool setTile(const TilePos&, TileID, TileChange updateFlags = TileChange::UPDATE_ALL);
	bool setTileAndDataNoUpdate(const TilePos&, const FullTile&);
	bool setTileNoUpdate(const TilePos&, TileID);
	void removeTile(const TilePos&);
	void setBrightness(const LightLayer&, const TilePos&, Brightness_t);
	void setGrassColor(int, const TilePos&, int); // unk type
	void fetchCollisionShapes(Entity*, const AABB&, float*, bool); // unk type
	std::vector<AABB>& fetchAABBs(const AABB&, bool b = true); // unk type
	std::vector<Entity*> getEntities(const EntityType&, const AABB&, Entity*) const; // unk type
	void getNearestEntityOfType(Entity*, const AABB&, const EntityType&) const; // unk type
	void getNearestEntityOfType(Entity*, const Vec3&, float, const EntityType&) const; // unk type
	const std::vector<Entity*>& getEntities(Entity* except, const AABB& bb) const;
	bool isUnobstructedByEntities(const AABB&, Entity*) const;
	bool mayInteract(Entity* entity, const TilePos& pos) const;
	bool mayPlace(TileID tileId, const TilePos& pos, bool ignoreEntities = false) const;
	bool mayPlace(TileID tileId, const TilePos& pos, Facing::Name face, Entity& placer, bool ignoreEntities = false, Entity* ignoreEntity = nullptr) const;
	void addListener(TileSourceListener& listener);
	void removeListener(TileSourceListener& listener);

protected:
	std::thread::id m_threadId;
	bool m_bAllowUnpopulatedChunks;
	bool m_bPublicSource;
	Level& m_level;
	ChunkSource& m_chunkSource;
	Dimension& m_dimension;
	Listeners m_listeners;
	LevelChunk* m_lastChunk;
	TileTickingQueue* m_tileTickingQueue;
};
