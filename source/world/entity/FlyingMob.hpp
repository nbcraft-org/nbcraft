#pragma once

#include "Mob.hpp"
#include "world/level/Level.hpp"

class FlyingMob : public Mob
{
public:
	FlyingMob(Level* pLevel);
	void causeFallDamage(float) override { return; }
	void travel(const Vec2& pos) override;
	bool onLadder() const override { return false; };
};
