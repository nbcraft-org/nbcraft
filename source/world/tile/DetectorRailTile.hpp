#pragma once

#include "RailTile.hpp"

#define C_POWERED_BIT 8

class DetectorRailTile : public RailTile
{
public:
	DetectorRailTile(TileID id, int texture);

private:
	void _setStateIfMinecartInteractsWithRail(Level* level, const TilePos& pos, TileData data) const;

public:
	int getTickDelay() const override;
	int getDirectSignal(const Level*, const TilePos& pos, Facing::Name face) const override;
	int getSignal(const TileSource*, const TilePos& pos, Facing::Name face) const override;
	bool isSignalSource() const override;
	virtual void entityInside(Level*, const TilePos& pos, Entity*) const override;
	virtual void tick(Level*, const TilePos& pos, Random*) override;

	// @NOTE: inlined
	inline static bool isPowered(TileData data)
	{
		return (data & C_POWERED_BIT) != 0;
	}
};
