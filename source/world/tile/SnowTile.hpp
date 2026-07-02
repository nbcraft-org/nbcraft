#pragma once

#include "Tile.hpp"

class SnowTile : public Tile
{
public:
	SnowTile(TileID id, int texture);

	int getResource(TileData data, Random*) const override;
	int getResourceCount(Random*) const override;
	void tick(TileSource&, const TilePos& pos, Random*) override;
};
