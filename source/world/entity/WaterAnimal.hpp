#pragma once

#include "PathfinderMob.hpp"

class WaterAnimal : public PathfinderMob
{
public:
	WaterAnimal(Level* pLevel);
	virtual bool isWaterMob() const override;
	void addAdditionalSaveData(CompoundTag& tag) const override;
	void readAdditionalSaveData(const CompoundTag& tag) override;
	bool canSpawn() override;
	int getAmbientSoundInterval() const override;

private:
	int field_BB4;
	int m_age;
};
