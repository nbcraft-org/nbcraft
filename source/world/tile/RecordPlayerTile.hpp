#pragma once

#include "EntityTile.hpp"

class RecordPlayerTile : public EntityTile
{
public:
	RecordPlayerTile(TileID id, int texture);

	int getTexture(Facing::Name face) const override;
	bool use(Level* level, const TilePos& pos, Player* player) override;
	void spawnResources(Level*, const TilePos& pos, TileID, float) override;
	void onRemove(Level* level, const TilePos& pos) override;
	static void playRecord(Level* level, const TilePos& pos, int);
	static void ejectRecord(Level* level, const TilePos& pos);
	TileEntity* newTileEntity() override;
};
