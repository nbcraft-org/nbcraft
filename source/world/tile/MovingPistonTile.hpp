#pragma once

#include "EntityTile.hpp"

class PistonMovingTileEntity;

class MovingPistonTile : public EntityTile
{
public:
	MovingPistonTile(TileID id);

	void onPlace(Level*, const TilePos& pos) override;
	void onRemove(Level*, const TilePos& pos) override;
	bool mayPlace(const Level*, const TilePos& pos) const override;
	//bool mayPlace(const Level*, const TilePos& pos, Facing::Name face) const override;
	eRenderShape getRenderShape() const override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	bool use(Level*, const TilePos& pos, Player*) override;
	int getResource(TileData, Random*) override;
	void spawnResources(Level*, const TilePos& pos, TileData, float) override;
	AABB* getAABB(const Level*, const TilePos& pos) override;
	void updateShape(const LevelSource*, const TilePos&) override;
	TileEntity* newTileEntity() override;

	AABB* moveByPositionAndProgress(const Level*, const TilePos& pos, TileID id, float progress, Facing::Name dir);
	static PistonMovingTileEntity* newMovingTileEntity(TileID tile, int meta, Facing::Name direction, bool extending, bool isSourcePiston);
};
