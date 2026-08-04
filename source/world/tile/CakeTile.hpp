#pragma once

#include "Tile.hpp"

class CakeTile : public Tile
{
public:
	CakeTile(int id, int texture);

public:
	void updateShape(const TileSource& source, const TilePos& pos) override;
	void updateDefaultShape() override;
	AABB* getAABB(const TileSource& source, const TilePos& pos) override;
	AABB getTileAABB(TileSource& source, const TilePos& pos) override;
public:
	int getTexture(Facing::Name face, TileData data) const override;
	int getTexture(Facing::Name face) const override;
private:
	void eat(const TilePos& pos, Player& player);
public:
	bool isCubeShaped() const override { return false;}
	bool isSolidRender() const override { return false; }
	bool use(const TilePos& pos, Player& player) override;
	bool mayPlace(const TileSource& source, const TilePos& pos) const override;
	bool canSurvive(const TileSource& source, const TilePos& pos) const override;
public:
	void neighborChanged(TileSource& source, const TilePos& pos, TileID tile) override;
	int getResource(TileData, Random*) const override { return TILE_AIR; }
	int getResourceCount(Random*) const override { return 0; }
};
