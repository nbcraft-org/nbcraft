#pragma once

#include <set>
#include "Tile.hpp"

class RedStoneDustTile : public Tile
{
public:
	RedStoneDustTile(TileID id, int texture);

	int getTexture(Facing::Name face, TileData data) const override;
	AABB* getAABB(TileSource& source, const TilePos& pos) override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	eRenderShape getRenderShape() const override;
	bool mayPlace(TileSource& source, const TilePos& pos) const override;
	void onPlace(TileSource& source, const TilePos& pos) override;
	void onRemove(TileSource& source, const TilePos& pos) override;
	void neighborChanged(TileSource& source, const TilePos& pos, TileID tile) override;
	int getResource(TileData data, Random* random) const override;
	int getDirectSignal(TileSource& source, const TilePos& pos, Facing::Name face) const override;
	int getSignal(TileSource& source, const TilePos& pos, Facing::Name face) const override;
	bool isSignalSource() const override;
	void animateTick(TileSource& source, const TilePos& pos, Random*) override;

	void updatePowerStrength(TileSource& source, const TilePos& pos);
	void updatePowerStrength(TileSource& source, const TilePos& pos1, const TilePos& pos2);
	void checkCornerChangeAt(TileSource& source, const TilePos& pos);
	int checkTarget(TileSource& source, const TilePos& pos, int);
	static bool shouldConnectTo(TileSource& source, const TilePos& pos);

public:
	std::set<TilePos> m_toUpdate;
	bool m_bShouldSignal;
};
