#pragma once

#include "Tile.hpp"

class PressurePlateTile : public Tile
{
public:
	enum Sensitivity
	{
		SENSITIVITY_EVERYTHING,
		SENSITIVITY_MOBS,
		SENSITIVITY_PLAYERS
	};

public:
	PressurePlateTile(TileID id, int texture, Sensitivity sensitivity);

public:
	int getTickDelay() const override;
	AABB* getAABB(const TileSource&, const TilePos& pos) override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	bool mayPlace(const TileSource&, const TilePos& pos) const override;
	void onPlace(TileSource&, const TilePos& pos) override;
	void neighborChanged(TileSource&, const TilePos& pos, TileID tile) override;
	void tick(TileSource&, const TilePos& pos, Random*) override;
	void entityInside(TileSource&, const TilePos& pos, Entity*) const override;
	void onRemove(TileSource&, const TilePos& pos) override;
	void updateShape(const TileSource&, const TilePos& pos) override;
	int getSignal(const TileSource&, const TilePos& pos, Facing::Name face) const override;
	int getDirectSignal(const TileSource&, const TilePos& pos, Facing::Name face) const override;
	bool isSignalSource() const override;
	void updateDefaultShape() override;

	void checkPressed(TileSource&, const TilePos& pos) const;

public:
	Sensitivity m_sensitivity;
};
