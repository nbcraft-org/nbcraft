#pragma once

#include "EntityTile.hpp"

class MusicTile : public EntityTile
{
public:
    MusicTile(TileID id, int textureID);

public:
	void neighborChanged(TileSource&, const TilePos& pos, TileID tile) override;
	bool use(const TilePos&, Player&) override;
	void attack(const TilePos&, Player&) override;
	bool hasTileEntity() const override;
	void triggerEvent(TileSource&, const TileEvent&) override;
	TileEntity* newTileEntity() override;
};