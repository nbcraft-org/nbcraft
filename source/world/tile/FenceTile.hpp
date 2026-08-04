#pragma once

#include "Tile.hpp"

class FenceTile : public Tile
{
public:
	FenceTile(int id, int texture);
	bool mayPlace(const TileSource&, const TilePos& pos) const override;
	AABB* getAABB(const TileSource& source, const TilePos& pos) override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	eRenderShape getRenderShape() const override;
	bool shouldConnectTo(TileSource& source, const TilePos& pos) const;
};
