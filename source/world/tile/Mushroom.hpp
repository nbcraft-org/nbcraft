#pragma once

#include "Bush.hpp"

class Mushroom : public Bush
{
public:
	Mushroom(TileID id, int texture);

	void tick(Level* pLevel, const TilePos& pos, Random* pRandom) override;
	bool canSurvive(const Level*, const TilePos& pos) const override;
	bool isValidGrowTile(const TileID tile) const override;
};
