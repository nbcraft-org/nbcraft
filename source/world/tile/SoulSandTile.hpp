#pragma once

#include "Tile.hpp"

class SoulSandTile : public Tile
{
public:
	SoulSandTile(int id, int texture);

public:
	void entityInside(TileSource& source, const TilePos& pos, Entity* entity) const override;
	AABB* getAABB(const TileSource& source, const TilePos& pos) override;
};
