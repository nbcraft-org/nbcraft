#pragma once

#include "Tile.hpp"

class PressurePlateTile : public Tile
{
public:
	enum Sensitivity
	{
		everything,
		mobs,
		players
	};

public:
	PressurePlateTile(TileID id, int texture, Sensitivity sensitivity);

	int getTickDelay() const override;
	AABB* getAABB(const Level*, const TilePos& pos) override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	bool mayPlace(const Level*, const TilePos& pos) const override;
	void onPlace(Level*, const TilePos& pos) override;
	void neighborChanged(Level* level, const TilePos& pos, TileID tile) override;
	void tick(Level*, const TilePos& pos, Random*) override;
	void entityInside(Level*, const TilePos& pos, Entity*) const override;
	void onRemove(Level*, const TilePos& pos) override;
	void updateShape(const LevelSource*, const TilePos& pos) override;
	int getSignal(const LevelSource*, const TilePos& pos, Facing::Name face) const override;
	int getDirectSignal(const Level*, const TilePos& pos, Facing::Name face) const override;
	bool isSignalSource() const override;
	void updateDefaultShape() override;

	void checkPressed(Level*, const TilePos& pos) const;

public:
	Sensitivity m_sensitivity;

};