#pragma once

#include "Tile.hpp"

class RecordPlayerTile : public Tile
{
public:
	RecordPlayerTile(TileID id, int texture);

public:
	int getTexture(Facing::Name face) const override;
	bool use(Level*, const TilePos& pos, Player*) override;
	void dropRecording(Level*, const TilePos& pos, TileData data);
	void spawnResources(Level*, const TilePos& pos, TileData data, float) override;
};
