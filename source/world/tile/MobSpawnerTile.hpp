#pragma once

#include "EntityTile.hpp"

class MobSpawnerTile : public EntityTile
{
public:
	MobSpawnerTile(TileID id, int texture);

	TileEntity* newTileEntity() override;
	int getResource(TileData, Random*) const override;
	int getResourceCount(Random*) const override;
	bool isSolidRender() const override;
};
