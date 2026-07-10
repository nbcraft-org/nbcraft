#pragma once
#include "client/renderer/LightLayer.hpp"
#include "world/entity/EntityType.hpp"
#include "world/entity/MobSpawnerData.hpp"
#include "world/level/levelgen/biome/Biome.hpp"
#include "world/level/levelgen/chunk/ChunkPos.hpp"
#include "world/level/Brightness.hpp"
#include "world/level/TileChange.hpp"
#include "world/tile/Tile.hpp"
#include "world/tile/entity/TileEntity.hpp"

class TileTickingQueue;
class AABB;
class LevelChunk;
class TileSourceListener; // TODO
class TileTickingQueue;
class Dimension;
class ChunkSource;
class BiomeSource;
struct Bounds;

typedef std::vector<AABB> AABBVector;

/*class LevelSource
{
public:
	virtual ~LevelSource() = 0;
	virtual TileID getTile(const TilePos& pos) const = 0;
	virtual float getBrightness(const TilePos& pos) const = 0;
	virtual TileData getData(const TilePos& pos) const = 0;
	virtual Material* getMaterial(const TilePos& pos) const = 0;
	virtual bool isSolidTile(const TilePos& pos) const = 0;
	virtual BiomeSource* getBiomeSource() const = 0;
	virtual TileEntity* getTileEntity(const TilePos& pos) const = 0;
};*/

class TileSource
{
protected:
	typedef std::vector<TileSourceListener*> Listeners;

public:
	//TileSource(Level& level, Dimension& dimension, ChunkSource& source, bool publicSource, bool allowUnpopulatedChunks);
	virtual ~TileSource();

public:
	virtual Level& getLevel() const = 0;
	virtual const Level& getLevelConst() const = 0;
	virtual Dimension& getDimension() const = 0;
	virtual const Dimension& getDimensionConst() const = 0;
	virtual ChunkSource& getChunkSource() const = 0;
	virtual void setTickingQueue(TileTickingQueue& queue) = 0;
	virtual TileTickingQueue* getTickQueue(const TilePos& pos) const = 0;
	virtual LevelChunk* getChunk(const ChunkPos& pos) const = 0;
	//virtual LevelChunk* getWritableChunk(const ChunkPos& pos) = 0;
	//virtual bool shouldFireEvents(LevelChunk& chunk) const = 0;
	virtual bool hasChunk(const ChunkPos& pos) const = 0; // from Level
	virtual bool hasChunkAt(const TilePos& pos) const = 0;
	//virtual bool hasChunksAt(const Bounds& bounds) const = 0;
	//virtual bool hasChunksAt(const AABB& aabb) const = 0;
	virtual bool hasChunksAt(const TilePos& min, const TilePos& max) const = 0;
	virtual bool hasChunksAt(const TilePos& pos, int r) const = 0;
	virtual Brightness_t getSkyDarken() const = 0;
	//virtual const std::vector<MobSpawnerData>& getMobsAt(EntityType entityType, const TilePos& pos) = 0;
	virtual int getSignal(const TilePos& pos, Facing::Name face) const = 0;
	virtual int getDirectSignal(const TilePos& pos, Facing::Name face) const = 0;
	virtual bool hasDirectSignal(const TilePos& pos) const = 0;
	virtual bool hasNeighborSignal(const TilePos& pos) const = 0;
	//virtual void runLightUpdates(const LightLayer&, const TilePos&, const TilePos&) = 0; // unk type
	//virtual bool isTopSolidBlocking(Tile*, uint8_t) = 0;
	//virtual void onChunkDiscarded(LevelChunk& chunk) = 0;
	//virtual void clearCachedLastChunk() = 0; // unk type
	//virtual void fireAreaChanged(const TilePos&, const TilePos&) = 0; // unk type
	virtual void fireTilesDirty(const TilePos&, const TilePos&) = 0; // unk type
	//virtual void fireTileChanged(const TilePos&, const FullTile&, const FullTile&, TileChange) = 0;
	//virtual void fireBrightnessChanged(const TilePos&) = 0; // unk type
	//virtual void fireTileEntityRemoved(std::unique_ptr<TileEntity>&) = 0; // unk type
	//virtual void fireTileEvent(int, int, int, int, int) = 0; // unk type
	virtual LevelChunk* getChunkAt(const TilePos&) const = 0;
	//virtual void hasTile(const TilePos&) = 0; // unk type
	//virtual void fireTileEntityChanged(TileEntity&) = 0; // unk type
	//virtual bool isOwnerThread() const = 0;
	virtual DimensionId getDimensionId() const = 0;
	//virtual TileData setExtraData(const TilePos&, uint16_t) = 0; // unk type
	//virtual TileData getExtraData(const TilePos&) = 0; // unk type
	virtual TileID getTopSolidBlock(const TilePos&, bool b = false) const = 0; // @TODO: check the default value on the bool
	virtual bool canSeeSky(const TilePos&) const = 0;
	virtual uint8_t getHeightmap(const TilePos&) const = 0;
	//virtual void getHeightmapPos(const TilePos&) const = 0; // unk type
	virtual TileEntity* getTileEntity(const TilePos& pos) const = 0;
	virtual const TileEntity::Vector& getAllTileEntities() const = 0;
	virtual void setTileEntity(const TilePos& pos, TileEntity* tileEntity) = 0;
	virtual void removeTileEntity(const TilePos& pos) = 0;
	//virtual void getGrassColor(const TilePos&) = 0; // unk type
	//virtual void getTopRainTilePos(const TilePos&) = 0; // unk type
	//virtual void checkIsTopRainTilePos(const TilePos&) = 0; // unk type
	virtual TileID getTile(const TilePos& pos) const = 0;
	//virtual Tile* getTilePtr(const TilePos& pos) const = 0;
	virtual TileData getData(const TilePos& pos) const = 0;
	virtual bool isSolidBlockingTile(const TilePos&) const = 0;
	//virtual void isSolidRenderTile(const TilePos&) const = 0; // unk type
	virtual bool containsMaterial(const AABB&, const Material*) const = 0;
	virtual void tileEvent(const TileEvent& event) = 0; // unk type
	virtual bool containsAnyLiquid(const AABB&) const = 0;
	virtual bool containsFireTile(const AABB&) const = 0;
	virtual Material* getMaterial(const TilePos&) const = 0;
	//virtual void isWaterAt(const TilePos&) const = 0; // unk type
	//virtual void isIceAt(const TilePos&) const = 0; // unk type
	virtual void updateNeighborsAt(const TilePos&, TileID) = 0; // unk type
	virtual bool isEmptyTile(const TilePos&) const = 0;
	//virtual void findNextTopSolidTileUnder(TilePos&) = 0; // unk type
	//virtual void containsAny(const TilePos&, const TilePos&) = 0; // unk type
	virtual Brightness_t getRawBrightness(const TilePos& pos, bool b) const = 0;
	virtual Brightness_t getRawBrightness(const TilePos& pos) const { return getRawBrightness(pos, true); }
	virtual Brightness_t getBrightness(const LightLayer& lightLayer, const TilePos& pos) const = 0;
	virtual float getBrightness(const TilePos& pos) const = 0;
	virtual void updateLightIfOtherThan(const LightLayer&, const TilePos&, Brightness_t) = 0; // unk type
	virtual bool containsLiquid(const AABB&, const Material*) const = 0;
	//virtual FullTile getTileAndData(const TilePos&) const = 0; // unk type
	//virtual bool isTopSolidBlocking(const TilePos&) const = 0;
	virtual HitResult clip(const Vec3& A, const Vec3& B, bool liquid = false, bool solidOnly = false) const = 0;
	virtual float getSeenPercent(const Vec3&, const AABB&) const = 0;
	//virtual void getLightColor(const TilePos&, int) const = 0; // unk type
	virtual Biome& getBiome(const TilePos&) const = 0;
	//virtual bool shouldFreeze(const TilePos&, bool) = 0;
	//virtual bool shouldFreezeIgnoreNeighbors(const TilePos&) = 0;
	//virtual bool shouldThaw(const TilePos&, bool) = 0;
	//virtual bool isSnowTemperature(const TilePos&) = 0;
	//virtual bool isHumidAt(const TilePos&) = 0;
	virtual bool setTileAndData(const TilePos&, const FullTile&, TileChange updateFlags = TileChange::UPDATE_NONE) = 0;
	virtual bool setTile(const TilePos&, TileID, TileChange updateFlags = TileChange::UPDATE_NONE) = 0;
	virtual bool setTileAndDataNoUpdate(const TilePos&, const FullTile&) = 0;
	virtual bool setTileNoUpdate(const TilePos&, TileID) = 0;
	//virtual void removeTile(const TilePos&) = 0;
	virtual void setBrightness(const LightLayer&, const TilePos&, Brightness_t) = 0;
	//virtual void setGrassColor(int, const TilePos&, int) = 0; // unk type
	//virtual void fetchCollisionShapes(Entity*, const AABB&, float*, bool) = 0; // unk type
	virtual AABBVector& fetchAABBs(const AABB&, bool b = true) = 0; // unk type
	virtual std::vector<Entity*> getEntities(const EntityType&, const AABB&, Entity*) const = 0; // unk type
	//virtual void getNearestEntityOfType(Entity*, const AABB&, const EntityType&) const = 0; // unk type
	//virtual void getNearestEntityOfType(Entity*, const Vec3&, float, const EntityType&) const = 0; // unk type
	//virtual const std::vector<Entity*>& getEntities(Entity* except, const AABB& bb) const = 0
	virtual std::vector<Entity*> getEntities(Entity* except, const AABB& bb) const = 0; // from Level
	virtual bool isUnobstructedByEntities(const AABB&, Entity*) const = 0;
	virtual bool mayInteract(Entity* entity, const TilePos& pos) const = 0;
	virtual bool mayPlace(TileID tileId, const TilePos& pos, bool ignoreEntities = false) const = 0;
	virtual bool mayPlace(TileID tileId, const TilePos& pos, Facing::Name face, Entity& placer, bool ignoreEntities = false, Entity* ignoreEntity = nullptr) const = 0;
	virtual void addListener(TileSourceListener& listener) = 0;
	virtual void removeListener(TileSourceListener& listener) = 0;

	// from LevelSource
	virtual BiomeSource* getBiomeSource() const = 0;
};
