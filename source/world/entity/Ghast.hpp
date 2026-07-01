#pragma once

#include "FlyingMob.hpp"

class Ghast : public FlyingMob
{
public:
	Ghast(Level* pLevel);

private:
	bool _canReach(const Vec3&, float);

public:
	void updateAi() override;
	std::string getAmbientSound() override { return "mob.ghast.moan"; }
	std::string getHurtSound() const override { return "mob.ghast.scream"; }
	std::string getDeathSound() const override { return "mob.ghast.death"; }
	int getDeathLoot() const override { return Item::sulphur->m_itemID; }
	float getSoundVolume() const override { return 10.0f; }
	bool canSpawn() override { return m_pLevel->m_difficulty > 0 && FlyingMob::canSpawn() && m_random.nextInt(20) == 0; }
	int getMaxSpawnClusterSize() const override { return 1; }

	int m_floatDuration;
	Vec3 m_targetPos;
	int m_oCharge;
	int m_charge;

private:
	Entity* m_target;
	int m_retargetTime;
};
