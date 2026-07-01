#include "WaterAnimal.hpp"
#include "world/level/TileSource.hpp"

WaterAnimal::WaterAnimal(TileSource& source) : PathfinderMob(source)
{
}

bool WaterAnimal::isWaterMob() const
{
	return true;
}

bool WaterAnimal::canSpawn()
{
	return m_pTileSource->isUnobstructedByEntities(m_hitbox, this);
}

int WaterAnimal::getAmbientSoundInterval() const
{
	return 120;
}
