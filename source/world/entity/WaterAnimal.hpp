#pragma once

#include "PathfinderMob.hpp"
#include "world/level/Level.hpp"

class WaterAnimal : public PathfinderMob
{
public:
	WaterAnimal(TileSource& source);

public:
	bool isWaterMob() const override;
	bool canSpawn() override;
	int getAmbientSoundInterval() const override;
};
