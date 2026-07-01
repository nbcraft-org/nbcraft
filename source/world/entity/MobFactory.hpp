#pragma once

#include "EntityType.hpp"
#include "Mob.hpp"

class MobCategory;

class MobFactory
{
public:
	struct SpawnData
	{
		int weight;

		SpawnData(int weight)
			: weight(weight)
		{
		}
		
		virtual bool canSpawn(Level& level, EntityType::ID id, const TilePos&) const { return true; }
	};

	struct SingleBiomeSpawnData : public SpawnData
	{
		Biome* biome;

		SingleBiomeSpawnData(Biome* biome, int weight)
			: SpawnData(weight)
			, biome(biome)
		{
		}

		bool canSpawn(Level& level, EntityType::ID id, const TilePos& tp) const override;
	};

	typedef HashMap<EntityType::ID, SpawnData*> SpawnDataMap;

	static void initMobLists();
	static Mob* CreateMob(EntityType::ID entityType, Level *level);
	static void addSpawnData(EntityCategories::CategoriesMask category, EntityType::ID id, int weight);
	static void addSpawnData(EntityCategories::CategoriesMask category, EntityType::ID id, SpawnData*);
	static const SpawnDataMap& GetMobListOfCategory(const EntityCategories& category);
};
