#pragma once

#include "Tile.hpp"

class RecordPlayerTile : public Tile
{
public:
	RecordPlayerTile(TileID id, int texture);

public:
	int getTexture(Facing::Name face) const override;
	bool use(const TilePos& pos, Player&) override;
	void dropRecording(TileSource&, const TilePos& pos, TileData data);
	void spawnResources(TileSource&, const TilePos& pos, TileData data, float) override;
};
