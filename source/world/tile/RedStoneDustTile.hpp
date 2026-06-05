#pragma once

#include <set>
#include "Tile.hpp"

class RedStoneDustTile : public Tile
{
public:
	RedStoneDustTile(TileID id, int texture);

	int getTexture(Facing::Name face, TileData data) const override;
	AABB* getAABB(const Level*, const TilePos& pos) override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	eRenderShape getRenderShape() const override;
	bool mayPlace(const Level*, const TilePos& pos) const override;
	void onPlace(Level*, const TilePos& pos) override;
	void onRemove(Level*, const TilePos& pos) override;
	void neighborChanged(Level* level, const TilePos& pos, TileID tile) override;
	int getResource(TileData data, Random* random) const override;
	int getDirectSignal(const Level*, const TilePos& pos, Facing::Name face) const override;
	int getSignal(const LevelSource*, const TilePos& pos, Facing::Name face) const override;
	bool isSignalSource() const override;
	void animateTick(Level*, const TilePos& pos, Random*) override;

	void updatePowerStrength(Level*, const TilePos& pos);
	void updatePowerStrength(Level*, const TilePos& pos1, const TilePos& pos2);
	void checkCornerChangeAt(Level*, const TilePos& pos);
	int checkTarget(Level*, const TilePos& pos, int);
	static bool shouldConnectTo(const LevelSource*, const TilePos& pos);

public:
	std::set<TilePos> m_toUpdate;
	bool m_bShouldSignal;
};
