#pragma once

#include "RailTile.hpp"

class DetectorRailTile : public RailTile
{
public:
	DetectorRailTile(TileID id, int texture);

private:
	void _setStateIfMinecartInteractsWithRail(TileSource& source, const TilePos& pos, TileData data) const;

public:
	int getTickDelay() const override;
	int getDirectSignal(const TileSource&, const TilePos& pos, Facing::Name face) const override;
	int getSignal(const TileSource&, const TilePos& pos, Facing::Name face) const override;
	bool isSignalSource() const override;
	virtual void entityInside(TileSource&, const TilePos& pos, Entity*) const override;
	virtual void tick(TileSource&, const TilePos& pos, Random*) override;

	// @NOTE: inlined
	inline static bool isPowered(TileData data)
	{
		return (data & C_POWERED_BIT) != 0;
	}
};
