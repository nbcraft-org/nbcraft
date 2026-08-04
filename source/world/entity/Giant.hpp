#pragma once

#include "Monster.hpp"

class Giant : public Monster
{
public:
	Giant(TileSource& source);

public:
	float getWalkTargetValue(const TilePos& pos) const override;
};
