#include "WaterAnimal.hpp"
#include "world/level/Level.hpp"
#include "nbt/CompoundTag.hpp"

WaterAnimal::WaterAnimal(Level* pLevel) : PathfinderMob(pLevel)
{
}

bool WaterAnimal::isWaterMob() const
{
	return true;
}

void WaterAnimal::addAdditionalSaveData(CompoundTag& tag) const
{
	Mob::addAdditionalSaveData(tag);
}

void WaterAnimal::readAdditionalSaveData(const CompoundTag& tag)
{
	Mob::readAdditionalSaveData(tag);
}

bool WaterAnimal::canSpawn()
{
	return m_pLevel->isUnobstructed(&m_hitbox);
}

int WaterAnimal::getAmbientSoundInterval() const
{
	return 120;
}
