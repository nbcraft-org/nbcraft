#pragma once

#include "Mob.hpp"

class FlyingMob : public Mob
{
public:
	FlyingMob(TileSource& source);

public:
	void causeFallDamage(float) override { }
	void travel(const Vec2& pos) override;
	bool onLadder() const override { return false; }
};
