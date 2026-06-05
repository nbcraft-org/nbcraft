#pragma once

#include "Monster.hpp"

class Giant : public Monster
{
public:
	Giant(Level* pLevel);

	float getWalkTargetValue(const TilePos& pos) const override;
};
