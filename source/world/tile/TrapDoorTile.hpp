#pragma once

#include "Tile.hpp"

#define C_DIR_MASK 3
#define C_OPEN_BIT 4

class TrapdoorTile : public Tile
{
public:
	TrapdoorTile(int ID, Material*);

public:
	bool isCubeShaped() const override { return false; }
	bool isSolidRender() const override { return false; }
public:
	AABB* getAABB(TileSource& source, const TilePos& pos) override;
	AABB getTileAABB(TileSource& source, const TilePos& pos) override;
	HitResult clip(TileSource& source, const TilePos& pos, Vec3, Vec3) override;
public:
	void updateShape(TileSource& source, const TilePos& pos) override;
	void updateDefaultShape() override;
	void attack(const TilePos& pos, Player&) override;
	void neighborChanged(TileSource& source, const TilePos& pos, TileID newTile) override;
	void setPlacedOnFace(TileSource& source, const TilePos& pos, Facing::Name face) override;
	void setOpen(TileSource& source, const TilePos& pos, bool bOpen);
	bool use(const TilePos& pos, Player&) override;
	bool mayPlace(TileSource& source, const TilePos& pos) const override;
	bool blocksLight() const;
	
	// @NOTE: These are inlined.
	inline static bool isOpen(TileData data)
	{
		return (data & C_OPEN_BIT) != 0;
	}
};
