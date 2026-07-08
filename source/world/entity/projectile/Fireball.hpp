#pragma once

#include "Snowball.hpp"

class Fireball : public Snowball
{
private:
	void _init();

public:
	Fireball(Level* pLevel);
	Fireball(Level* pLevel, Mob* pMob, Vec3 pos);

	void tick() override;
	void onHit() override;
	void readAdditionalSaveData(const CompoundTag& tag) override;
	bool isPickable() const override { return true; }
	float getPickRadius() const override { return 1.0f; }
	bool hurt(Entity* pCulprit, int damage) override;

public:
	Mob* m_owner;
};
