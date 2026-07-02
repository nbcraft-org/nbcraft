#pragma once

#include "FlyingMob.hpp"

class Ghast : public FlyingMob
{
public:
	Ghast(TileSource& source);

private:
	bool _canReach(const Vec3&, float);

public:
	void updateAi() override;
	std::string getAmbientSound() const override { return "mob.ghast.moan"; }
	std::string getHurtSound() const override { return "mob.ghast.scream"; }
	std::string getDeathSound() const override { return "mob.ghast.death"; }
	int getDeathLoot() const override { return Item::sulphur->m_itemID; }
	float getSoundVolume() const override { return 10.0f; }
	bool canSpawn() override;
	int getMaxSpawnClusterSize() const override { return 1; }

private:
	Entity* m_target;
	int m_retargetTime;

public:
	int m_floatDuration;
	Vec3 m_targetPos;
	int m_oCharge;
	int m_charge;
};
