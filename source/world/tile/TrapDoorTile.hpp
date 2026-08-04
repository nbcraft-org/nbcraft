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
	AABB* getAABB(const Level*, const TilePos& pos) override;
	AABB getTileAABB(const Level*, const TilePos& pos) override;
	HitResult clip(const Level*, const TilePos& pos, Vec3, Vec3) override;
public:
	void updateShape(const LevelSource*, const TilePos& pos) override;
	void updateDefaultShape() override;
	void attack(Level*, const TilePos& pos, Player*) override;
	void neighborChanged(Level*, const TilePos& pos, TileID newTile) override;
	void setPlacedOnFace(Level* level, const TilePos& pos, Facing::Name face) override;
	void setOpen(Level*, const TilePos& pos, bool bOpen);
	bool use(Level*, const TilePos& pos, Player*) override;
	bool mayPlace(const Level* level, const TilePos& pos) const override;
	bool blocksLight() const;
	
	// @NOTE: These are inlined.
	inline static bool isOpen(TileData data)
	{
		return (data & C_OPEN_BIT) != 0;
	}
};
