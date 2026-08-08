/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include <set>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <map>

#include "common/utility/HashMap.hpp"
#include "client/renderer/LightUpdate.hpp"
#include "world/tile/Tile.hpp"
#include "world/tile/entity/TileEntity.hpp"
#include "world/entity/Entity.hpp"
#include "world/level/TileChange.hpp"
#include "world/level/TileSource.hpp"
#include "world/level/levelgen/chunk/LevelChunk.hpp"
#include "world/level/levelgen/chunk/ChunkSource.hpp"
#include "world/level/storage/LevelStorageSource.hpp"
#include "world/level/storage/LevelData.hpp"
#include "world/level/path/PathFinder.hpp"
#include "Dimension.hpp"
#include "LevelListener.hpp"
#include "TileTickingQueue.hpp"
#include "TickNextTickData.hpp"
#include "LevelEvent.hpp"

class Dimension;
class Level;
class LevelListener;
class RakNetInstance;
class Packet;
class MobSpawner;

typedef Entity::Vector EntityVector;
typedef HashMap<Entity::ID, Entity*> EntityMap;
typedef TileEntity::Vector TileEntityVector;
typedef std::vector<AABB> AABBVector;

class Level : public TileSource
{
public:
	Level(LevelStorage* pStor, const std::string& name, const LevelSettings& settings, int storageVersion = LEVEL_STORAGE_VERSION_DEFAULT, Dimension* pDimension = nullptr);
	~Level();

private:
	Player* _getNearestPlayer(const Vec3&, float, bool) const;
	bool _mayPlace(TileID tile, const TilePos& pos, bool ignoreEntities, Entity* ignoreEntity) const;
	void _resetWeatherCycle();

protected:
	// @NOTE: LevelListeners do NOT get updated here
	void _setTime(int32_t time) { m_pLevelData->setTime(time); }

public:
	// TODO
	TileID getTile(const TilePos& pos) const override;
	float getBrightness(const TilePos& pos) const override;
	TileData getData(const TilePos& pos) const override;
	Material* getMaterial(const TilePos& pos) const override;
	bool isSolidTile(const TilePos& pos) const override;
	bool isSolidBlockingTile(const TilePos& pos) const override;
	bool isSolidRenderTile(const TilePos&) const override;

	void toggleRain();
	float getThunderLevel(float) const;
	void setThunderLevel(float);
	float getRainLevel(float) const;
	void setRainLevel(float);
	bool isThundering() const;
	bool isRaining() const;
	bool isRainingAt(const TilePos&) const;

	ChunkSource& getChunkSource() const override;
	virtual ChunkSource* createChunkSource();
	LevelChunk* getChunk(const ChunkPos& pos) const override;
	LevelChunk* getChunkAt(const TilePos& pos) const override;
	Brightness_t getRawBrightness(const TilePos& pos, bool b) const override;
	Brightness_t getRawBrightness(const TilePos& pos) const { return getRawBrightness(pos, true); }
	TileEntity* getTileEntity(const TilePos& pos) const override;
	const TileEntity::Vector& getAllTileEntities() const override;
	void setTileEntity(const TilePos& pos, TileEntity* tileEntity) override;
	void removeTileEntity(const TilePos& pos) override;
	Brightness_t getBrightness(const LightLayer&, const TilePos& pos) const override;
	void setBrightness(const LightLayer&, const TilePos& pos, Brightness_t brightness) override;
	int getSeaLevel() const { return 63; }
	int getSeed() const { return m_pLevelData->getSeed(); }
	int32_t getTime() const { return m_pLevelData->getTime(); }
	void setTime(int32_t time);
	GameType getDefaultGameType() const { return m_pLevelData->getGameType(); }
	uint8_t getHeightmap(const TilePos& pos) const override;
	bool isDay() const;
	bool isSkyLit(const TilePos& pos) const override;
	bool canSeeSky(const TilePos& pos) const override;
	bool isEmptyTile(const TilePos& pos) const override;
	bool hasChunkAt(const TilePos& pos) const override;
	bool hasChunk(const ChunkPos& pos) const override;
	bool hasChunksAt(const TilePos& min, const TilePos& max) const override;
	bool hasChunksAt(const TilePos& pos, int rad) const override;
	float getTimeOfDay(float f) const;
	Brightness_t getSkyDarken() const override;
	int getSkyDarken(float f) const;
	void updateSkyDarken();
	bool updateSkyBrightness();
	void setUpdateLights(bool b);
	bool updateLights();
	void updateLight(const LightLayer&, const TilePos& lowerPos, const TilePos& upperPos);
	void updateLight(const LightLayer&, const TilePos& lowerPos, const TilePos& upperPos, bool);
	void updateLightIfOtherThan(const LightLayer&, const TilePos& pos, Brightness_t) override;
	bool setTileAndDataNoUpdate(const TilePos& pos, const FullTile& tile) override;
	bool setTileNoUpdate(const TilePos& pos, TileID tile) override;
	bool setDataNoUpdate(const TilePos& pos, TileData data);
	bool setTileAndData(const TilePos& pos, const FullTile& tile, TileChange updateFlags = TileChange::UPDATE_ALL) override;
	bool setTile(const TilePos& pos, TileID tile, TileChange updateFlags = TileChange::UPDATE_ALL) override;
	bool setData(const TilePos& pos, TileData data, TileChange updateFlags = TileChange::UPDATE_ALL);
	void sendTileUpdated(const TilePos& pos);
	void tileUpdated(const TilePos& pos, TileID tile);
	void updateNeighborsAt(const TilePos& pos, TileID tile) override;
	void neighborChanged(const TilePos& pos, TileID tile);
	void setTilesDirty(const TilePos& min, const TilePos& max) { fireTilesDirty(min, max); }
	void fireTilesDirty(const TilePos& min, const TilePos& max) override;
	void entityAdded(Entity* pEnt);
	void entityRemoved(Entity* pEnt);
	void levelEvent(const LevelEvent& event);
	void tileEvent(const TileEvent& event) override;
	void lightColumnChanged(int x, int z, int y1, int y2);
	bool containsFireTile(const AABB&) const override;
	bool containsAnyLiquid(const AABB&) const override;
	bool containsLiquid(const AABB&, const Material *pMtl) const override;
	bool containsMaterial(const AABB&, const Material *pMtl) const override;
	bool checkAndHandleWater(const AABB&, const Material* pMtl, Entity* pEnt);
	const TilePos& getSharedSpawnPos() const;
	void validateSpawn();
	TileID getTopTile(const TilePos& pos) const;
	int getTopTileY(const TilePos& pos) const;
	TileID getTopSolidBlock(const TilePos& tilePos, bool b = false) const override;
	GameType getLoadedPlayerGameType() const;
	void loadPlayer(Player&);
	bool addEntity(Entity*);
	bool removeEntity(Entity*);
	void removeEntities(const EntityVector&);
	void removeAllPendingEntityRemovals();
	void prepare();
	void saveLevelData();
	void savePlayerData();
	void saveAllChunks();
	void saveGame();
	void loadEntities();
	void sendEntityData();
	void setInitialSpawn();
	void setSpawnPos(const TilePos& pos) { m_pLevelData->setSpawn(pos); }
	void setSpawnSettings(bool a, bool b) { }
	Vec3 getSkyColor(const Entity& entity, float f) const;
	Vec3 getFogColor(float f) const;
	Vec3 getCloudColor(float f) const;
	bool isUnobstructed(AABB*) const;
	bool isUnobstructedByEntities(const AABB&, Entity* exclude) const override;
	bool mayInteract(Entity* entity, const TilePos& pos) const override;
	bool mayPlace(TileID tileId, const TilePos& pos, bool ignoreEntities = false) const override;
	bool mayPlace(TileID tileId, const TilePos& pos, Facing::Name face, Entity& placer, bool ignoreEntities = false, Entity* ignoreEntity = nullptr) const override;
	void broadcastAll(Packet* packet);
	void broadcastToAllInRange(Packet* packet, const Vec3& pos, float range, Player* avoid = nullptr);
	void broadcastEntityEvent(const Entity& entity, Entity::EventType::ID eventId);
	void removeListener(LevelListener*);
	void addListener(LevelListener*);
	void addListener(LevelListener& listener) {}
	void removeListener(LevelListener& listener) {}
	void tick(Entity*, bool);
	void tick(Entity*);
	virtual void tick();
	void tickPendingTicks(bool b);
	void tickTiles();
	void tickEntities();
	void tickWeather();
	void addToTickNextTick(const TilePos& tilePos, TileID tileId, int delay);
	void takePicture(TripodCamera* pCamera, Entity* pOwner);
	void addParticle(const std::string& name, const Vec3& pos, const Vec3& dir = Vec3::ZERO);
	void playSound(Entity*, const std::string& name, float volume = 1.0f, float pitch = 1.0f);
	void playSound(const Vec3& pos, const std::string& name, float volume = 1.0f, float pitch = 1.0f);
	void playStreamingMusic(const std::string& name, const TilePos& pos);
	void animateTick(const TilePos& pos);
	float getSeenPercent(const Vec3&, const AABB&) const override;
	void explode(Entity*, const Vec3& pos, float power);
	void explode(Entity*, const Vec3& pos, float power, bool bIsFiery);
	void addEntities(const EntityVector& entities);
	void ensureAdded(Entity* entity);
	bool extinguishFire(Player* player, const TilePos& pos, Facing::Name face);
	int findPath(Path* path, Entity* ent1, Entity* ent2, float f) const;
	int findPath(Path* path, Entity* ent, const TilePos& pos, float f) const;
	int getLightDepth(const TilePos& pos) const;
	float getStarBrightness(float f) const;
	float getSunAngle(float f) const;
	void swap(const TilePos& pos1, const TilePos& pos2);

	HitResult clip(const Vec3& a, const Vec3& b, bool includeLiquid = false, bool includeInvisible = true) const override;
	Entity* getEntity(Entity::ID id) const;
	unsigned int getEntityCount(const EntityCategories&) const;
	const EntityMap* getAllEntities() const;
	EntityVector getEntities(Entity* pAvoid, const AABB&) const override;
	void getEntities(DimensionId dimensionId, const EntityType& type, const AABB& aabb, std::vector<Entity*>& output) const;
	EntityVector getEntitiesOfCategory(EntityCategories::CategoriesMask category, const AABB&) const;
	EntityVector getEntitiesOfType(EntityType type, const AABB&) const;
	Player* getPlayer(const std::string&) const;
	BiomeSource* getBiomeSource() const override;
	LevelStorage* getLevelStorage() const { return m_pLevelStorage; }
	Dimension* getDimension(DimensionId type) const;
	const LevelData* getLevelData() const { return m_pLevelData; }
	AABBVector& fetchAABBs(const AABB& aabb, bool b = true) override;
	AABBVector* getCubes(const Entity* pEnt, const AABB& aabb);
	std::vector<LightUpdate>* getLightsToUpdate();
	Player* getNearestPlayer(const Entity&, float) const;
	Player* getNearestPlayer(const Vec3& pos, float, bool) const;
	Player* getNearestAttackablePlayer(const Entity&, float) const;
	Player* getNearestAttackablePlayer(const Vec3& pos, float, const Entity*) const;

	// unused redstone stuff
	int getSignal(const TilePos& pos, Facing::Name face) const override;
	int getDirectSignal(const TilePos& pos, Facing::Name face) const override;
	bool hasDirectSignal(const TilePos& pos) const override;
	bool hasNeighborSignal(const TilePos& pos) const override;

#ifdef ENH_IMPROVED_SAVING
	void saveUnsavedChunks();
#endif

	Level& getLevel() const override { return *(Level*)this; }
	const Level& getLevelConst() const override { return *this; }
	Dimension& getDimension() const override { return *getDimension(DIMENSION_OVERWORLD); }
	const Dimension& getDimensionConst() const override { return *getDimension(DIMENSION_OVERWORLD); }
	void setTickingQueue(TileTickingQueue& queue) override;
	TileTickingQueue* getTickQueue(const TilePos& pos) const override;
	DimensionId getDimensionId() const override { return DIMENSION_OVERWORLD; }
	Biome& getBiome(const TilePos&) const override;
	Entity::Vector getEntities(const EntityType&, const AABB&, Entity*) const override;

private:
	LevelData* m_pLevelData;
	bool m_bUpdatingTileEntities;

protected:
	int m_randValue;
	int m_addend;
	int m_saveInterval;
	int m_delayUntilNextMoodSound;
	float m_oRainLevel;
	float m_rainLevel;
	float m_oThunderLevel;
	float m_thunderLevel;

public:
	AABBVector m_aabbs;
	bool m_bInstantTicking;
	bool m_bIsClientSide; // if the level is controlled externally by a server.
	bool m_bPostProcessing;
	EntityMap m_entities;
	std::vector<Player*> m_players;
	int m_skyDarken;
	int m_skyFlashTime;
	bool m_bNoNeighborUpdate;
	Dimension* m_pDimension;
    int m_difficulty; // @TODO: Difficulty enum
	Random m_random;
	RakNetInstance* m_pRakNetInstance;
	bool m_bCalculatingInitialSpawn;
	std::vector<LevelListener*> m_levelListeners;
	ChunkSource* m_pChunkSource;
	LevelStorage* m_pLevelStorage;
	EntityVector m_pendingEntityRemovals;
	TileTickingQueue m_tileTickingQueue;
	std::set<ChunkPos> m_chunksToUpdate;
	std::vector<LightUpdate> m_lightUpdates;
	bool m_bUpdateLights;
	int m_maxRecurse;
	uint8_t field_B0C;
	int field_B10;
	PathFinder* m_pPathFinder;
	MobSpawner* m_pMobSpawner;

	HashMap<uint32_t, int> m_entityCountsByCategory;
	TileEntityVector m_tileEntities;
	TileEntityVector m_pendingTileEntities;
};
