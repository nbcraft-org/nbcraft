#pragma once

#include "PathfinderMob.hpp"
#include "world/level/Level.hpp"

class WaterAnimal : public PathfinderMob
{
public:
	WaterAnimal(Level* pLevel);
	bool isWaterMob() const override;
	void addAdditionalSaveData(CompoundTag& tag) const override;
	void readAdditionalSaveData(const CompoundTag& tag) override;
	bool canSpawn() override;
	int getAmbientSoundInterval() const override;
};
