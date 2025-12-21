#pragma once

#include "Tile.hpp"

class LeverTile : public Tile
{
public:
	LeverTile(TileID id, int texture);

	AABB* getAABB(const Level*, const TilePos& pos) override;
	virtual bool isSolidRender() const override;
	virtual bool isCubeShaped() const override;
	virtual eRenderShape getRenderShape() const override;
	bool mayPlace(const Level*, const TilePos& pos) const override;
	void setPlacedOnFace(Level*, const TilePos& pos, Facing::Name face) override;
	void neighborChanged(Level* level, const TilePos& pos, TileID tile) override;
	void updateShape(const LevelSource*, const TilePos& pos) override;
	void attack(Level*, const TilePos& pos, Player*) override;
	bool use(Level*, const TilePos& pos, Player*) override;
	void onRemove(Level*, const TilePos& pos) override;
	virtual int getSignal(const LevelSource*, const TilePos& pos, Facing::Name face) const override;
	virtual int getDirectSignal(const Level*, const TilePos& pos, Facing::Name face) const override;
	virtual bool isSignalSource() const override;

	bool checkCanSurvive(Level*, const TilePos& pos);
};