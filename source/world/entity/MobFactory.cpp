#include "MobFactory.hpp"
#include "common/Logger.hpp"

#include "Chicken.hpp"
#include "Cow.hpp"
#include "Pig.hpp"
#include "Sheep.hpp"
#include "Zombie.hpp"
#include "Creeper.hpp"
#include "Skeleton.hpp"
#include "Spider.hpp"
#include "Slime.hpp"
#include "Giant.hpp"
#include "PigZombie.hpp"
#include "Squid.hpp"
#include "Ghast.hpp"
#include "Wolf.hpp"

#define ENTS ENT(CHICKEN, Chicken) \
             ENT(COW, Cow) \
             ENT(PIG, Pig) \
             ENT(SHEEP, Sheep) \
             ENT(ZOMBIE, Zombie) \
             ENT(CREEPER, Creeper) \
             ENT(SKELETON, Skeleton) \
             ENT(SPIDER, Spider) \
             ENT(GIANT, Giant) \
             ENT(SLIME, Slime) \
             ENT(GHAST, Ghast) \
             ENT(PIG_ZOMBIE, PigZombie) \
             ENT(SQUID, Squid) \
             ENT(WOLF, Wolf)

#define ENT(enumType, classType) case EntityType::enumType: return new classType(level);

// format: ID, spawnrate
MobFactory::SpawnDataMap nullCreatureList;
std::map<EntityCategories::CategoriesMask, MobFactory::SpawnDataMap> mobListsByCategory;

Mob* MobFactory::CreateMob(EntityType::ID entityType, Level *level)
{
    switch (entityType)
    {
        ENTS;
    default:
        LOG_W("Unknown mob type requested: %d", entityType);
        return nullptr;
    }
}

void MobFactory::addSpawnData(EntityCategories::CategoriesMask category, EntityType::ID id, int weight)
{
    addSpawnData(category, id, new SpawnData(weight));
}

void MobFactory::initMobLists() 
{
    // format: ID, spawnrate
    addSpawnData(EntityCategories::MONSTER, EntityType::SPIDER, 10);
    addSpawnData(EntityCategories::MONSTER, EntityType::ZOMBIE, 10);
    addSpawnData(EntityCategories::MONSTER, EntityType::SKELETON, 10);
    addSpawnData(EntityCategories::MONSTER, EntityType::CREEPER, 10);
    addSpawnData(EntityCategories::MONSTER, EntityType::SLIME, 10);
    //addSpawnData(EntityCategories::MONSTER, EntityType::GHAST, new SingleBiomeSpawnData(Biome::hell, 10));
    //addSpawnData(EntityCategories::MONSTER, EntityType::PIG_ZOMBIE, new SingleBiomeSpawnData(Biome::hell, 10));

    addSpawnData(EntityCategories::ANIMAL, EntityType::SHEEP, 12);
    addSpawnData(EntityCategories::ANIMAL, EntityType::PIG, 10);
    addSpawnData(EntityCategories::ANIMAL, EntityType::CHICKEN, 10);
    addSpawnData(EntityCategories::ANIMAL, EntityType::COW, 8);
    addSpawnData(EntityCategories::ANIMAL, EntityType::WOLF, new SingleBiomeSpawnData(Biome::taiga, 2));

    addSpawnData(EntityCategories::WATER_ANIMAL, EntityType::SQUID, 10);
}

void MobFactory::addSpawnData(EntityCategories::CategoriesMask category, EntityType::ID id, SpawnData* data)
{
    std::map<EntityCategories::CategoriesMask, SpawnDataMap>::iterator it = mobListsByCategory.find(category);
    if (it == mobListsByCategory.end())
    {
        SpawnDataMap map;
        map.insert(id, data);
        mobListsByCategory[category] = map;
    }
    else
    {
        SpawnDataMap& map = (*it).second;
        map.insert(id, data);
    }
}

const MobFactory::SpawnDataMap& MobFactory::GetMobListOfCategory(const EntityCategories& category)
{
    // not for composite categories. only for the base categories (MONSTER, ANIMAL, ...)
    std::map<EntityCategories::CategoriesMask, SpawnDataMap>::const_iterator it = mobListsByCategory.find(category.getCategoryMask());
    if (it != mobListsByCategory.end())
        return it->second;

    return nullCreatureList;
}

#undef ENT

bool MobFactory::SingleBiomeSpawnData::canSpawn(Level& level, EntityType::ID id, const TilePos& tp) const
{
    if (biome == level.getBiomeSource()->getBiomeAt(tp))
        return true;

    return false;
}
