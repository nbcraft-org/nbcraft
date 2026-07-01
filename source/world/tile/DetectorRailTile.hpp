#pragma once

#include "RailTile.hpp"

class DetectorRailTile : public RailTile
{
public:
	DetectorRailTile(TileID id, int texture);

	int getTickDelay() const override;
	int getDirectSignal(const Level*, const TilePos& pos, Facing::Name face) const override;
	int getSignal(const LevelSource*, const TilePos& pos, Facing::Name face) const override;
	bool isSignalSource() const override;
	virtual void entityInside(Level*, const TilePos& pos, Entity*) const override;
	virtual void tick(Level*, const TilePos& pos, Random*) override;

private:
	void setStateIfMinecartInteractsWithRail(Level* level, const TilePos& pos, int data) const;

};