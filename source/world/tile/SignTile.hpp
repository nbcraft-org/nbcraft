#pragma once

#include "EntityTile.hpp"

class SignTile : public EntityTile
{
public:
	SignTile(TileID id, bool isWall);

	void neighborChanged(Level* level, const TilePos& pos, TileID tile) override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	virtual AABB* getAABB(const Level*, const TilePos& pos) override;
	eRenderShape getRenderShape() const override;
	void updateShape(const LevelSource* level, const TilePos& pos) override;
	int getResource(TileData, Random*) const override;
	bool hasTileEntity() const override;
	TileEntity* newTileEntity() override;

public:
	bool m_bIsWall;
};

