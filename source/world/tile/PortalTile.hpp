#pragma once

#include "HalfTransparentTile.hpp"

class PortalTile : public HalfTransparentTile
{
public:
	PortalTile(TileID ID, int texture);

	AABB* getAABB(const Level*, const TilePos& pos) override;
	void updateShape(const LevelSource*, const TilePos& pos) override;
	bool isCubeShaped() const override;
	bool trySpawnPortal(Level* level, const TilePos& pos);
	int getResourceCount(Random*) const override;
	void neighborChanged(Level*, const TilePos& pos, TileID newTile) override;
	void entityInside(Level*, const TilePos& pos, Entity*) const override;
	bool shouldRenderFace(const LevelSource* level, const TilePos& pos, Facing::Name face) const override;
	void animateTick(Level* level, const TilePos& pos, Random* random) override;
};
