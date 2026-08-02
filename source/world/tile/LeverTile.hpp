#pragma once

#include "Tile.hpp"

class LeverTile : public Tile
{
public:
	LeverTile(TileID id, int texture);

private:
	bool _checkCanSurvive(TileSource& source, const TilePos& pos);

public:
	AABB* getAABB(TileSource& source, const TilePos& pos) override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	eRenderShape getRenderShape() const override;
	bool mayPlace(TileSource& source, const TilePos& pos) const override;
	void setPlacedOnFace(TileSource& source, const TilePos& pos, Facing::Name face) override;
	void neighborChanged(TileSource& source, const TilePos& pos, TileID tile) override;
	void updateShape(TileSource& source, const TilePos& pos) override;
	void attack(const TilePos& pos, Player&) override;
	bool use(const TilePos& pos, Player&) override;
	void onRemove(TileSource& source, const TilePos& pos) override;
	int getSignal(TileSource& source, const TilePos& pos, Facing::Name face) const override;
	int getDirectSignal(TileSource& source, const TilePos& pos, Facing::Name face) const override;
	bool isSignalSource() const override;

};
