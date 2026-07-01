#pragma once

#include "EntityTile.hpp"

class FurnaceTile : public EntityTile
{
public:
	FurnaceTile(int id, bool active);

public:
	int getTexture(TileSource& source, const TilePos& pos, Facing::Name face) const override;
	void animateTick(TileSource& source, const TilePos& pos, Random* random) override;
	int getTexture(Facing::Name face) const override;
	void onPlace(TileSource& source, const TilePos& pos) override;
	bool use(const TilePos& pos, Player& player) override;
	void setPlacedBy(TileSource& source, const TilePos& pos, Mob*) override;
	void onRemove(TileSource& source, const TilePos& pos) override;
	bool hasTileEntity() const override;
	TileEntity* newTileEntity() override;
	int getResource(TileData, Random*) const override;

public:
	static void SetLit(bool lit, TileSource& source, const TilePos& pos);
	static void RecalculateLookDirection(TileSource& source, const TilePos& pos);

private:
	Random m_random;
	static bool keepInventory;

public:
	bool m_active;
};

