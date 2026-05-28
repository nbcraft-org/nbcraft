#pragma once

#include "Tile.hpp"

#define C_DIR_MASK 3
#define C_OPEN_BIT 4
#define C_CLOSE_MASK -5

class FenceGateTile : public Tile
{
public:
	FenceGateTile(int id, int texture);
public:
	void setPlacedBy(Level* pLevel, const TilePos& pos, Mob* mob) override;
	AABB* getAABB(const Level* pLevel, const TilePos& pos) override;
	eRenderShape getRenderShape() const override;
public:
	bool mayPlace(const Level*, const TilePos& pos) const override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	bool use(Level* pLevel, const TilePos& pos, Player* player) override;

	int getDir(TileData data) const;

	// @NOTE: These are inlined.
	inline static bool isOpen(TileData data)
	{
		return (data & C_OPEN_BIT) != 0;
	}
};
