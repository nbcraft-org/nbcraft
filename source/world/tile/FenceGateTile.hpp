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
	void setPlacedBy(const TilePos& pos, Mob& mob) override;
	AABB* getAABB(const TileSource& source, const TilePos& pos) override;
	eRenderShape getRenderShape() const override;

public:
	bool mayPlace(const TileSource&, const TilePos& pos) const override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	bool use(const TilePos& pos, Player& player) override;
	void neighborChanged(TileSource&, const TilePos& pos, TileID newTile) override;

	void setOpen(TileSource& source, const TilePos& pos, bool bOpen, Player* pPlayer = nullptr);

	static bool isOpen(TileData data)
	{
		return (data & C_OPEN_BIT) != 0;
	}

	static int getDir(TileData data)
	{
		return data & C_DIR_MASK;
	}
};
