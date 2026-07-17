#pragma once

#include "Tile.hpp"

class PistonHeadTile : public Tile
{
public:
	PistonHeadTile(TileID id, int texture);

	void onRemove(Level*, const TilePos& pos) override;
	eRenderShape getRenderShape() const override;
	int getTexture(Facing::Name side, TileData meta) const override;
	void neighborChanged(Level*, const TilePos& pos, TileID tile) override;
	void updateShape(const LevelSource*, const TilePos&) override;
	void addAABBs(const Level* level, const TilePos& pos, const AABB* aabb, std::vector<AABB>& out) override;

	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	bool mayPlace(const Level*, const TilePos& pos) const override;
	//bool mayPlace(const Level*, const TilePos& pos, Facing::Name face) const override;
	int getResourceCount(Random*) const override;

	void setHeadTexture(int texture);
	void resetHeadTexture();

	static int getDirection(int data)
	{
		return data & 7;
	}

private:
	int m_headTex;
};

