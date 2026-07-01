#pragma once

#include "Tile.hpp"

class LeverTile : public Tile
{
public:
	LeverTile(TileID id, int texture);

private:
	bool _checkCanSurvive(Level*, const TilePos& pos);

public:
	AABB* getAABB(const Level*, const TilePos& pos) override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	eRenderShape getRenderShape() const override;
	bool mayPlace(const Level*, const TilePos& pos) const override;
	void setPlacedOnFace(Level*, const TilePos& pos, Facing::Name face) override;
	void neighborChanged(Level* level, const TilePos& pos, TileID tile) override;
	void updateShape(const LevelSource*, const TilePos& pos) override;
	void attack(Level*, const TilePos& pos, Player*) override;
	bool use(Level*, const TilePos& pos, Player*) override;
	void onRemove(Level*, const TilePos& pos) override;
	int getSignal(const LevelSource*, const TilePos& pos, Facing::Name face) const override;
	int getDirectSignal(const Level*, const TilePos& pos, Facing::Name face) const override;
	bool isSignalSource() const override;

};
