#pragma once

#include "Tile.hpp"

class CakeTile : public Tile
{
public:
	CakeTile(int id, int texture);

public:
	void updateShape(const LevelSource*, const TilePos& pos) override;
	void updateDefaultShape() override;
	AABB* getAABB(const Level* pLevel, const TilePos& pos) override;
	AABB getTileAABB(const Level* pLevel, const TilePos& pos) override;
public:
	int getTexture(Facing::Name face, TileData data) const override;
	int getTexture(Facing::Name face) const override;
private:
	void eat(Level* pLevel, const TilePos& pos, Player* player);
public:
	bool isCubeShaped() const override { return false;}
	bool isSolidRender() const override { return false; }
	bool use(Level* pLevel, const TilePos& pos, Player* player) override;
	bool mayPlace(const Level* level, const TilePos& pos) const override;
	bool canSurvive(const Level* pLevel, const TilePos& pos) const override;
public:
	void neighborChanged(Level* pLevel, const TilePos& pos, TileID tile) override;
	int getResource(TileData, Random*) const override { return TILE_AIR; }
	int getResourceCount(Random*) const override { return 0; }
};
