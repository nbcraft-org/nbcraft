#pragma once

#include "Snowball.hpp"

class ThrownEgg : public Snowball
{
private:
	void _init();
public:
	ThrownEgg(Level* pLevel);
	ThrownEgg(Level* pLevel, Mob* pMob);
	ThrownEgg(Level* pLevel, const Vec3& pos, bool isPlayerOwned = false);

public:
	void onHit() override;
};
