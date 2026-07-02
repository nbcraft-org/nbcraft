#pragma once

#include "Tile.hpp"

class ButtonTile : public Tile
{
public:
	ButtonTile(TileID id, int texture);

public:
	AABB* getAABB(TileSource&, const TilePos& pos) override;
	int getTickDelay() const override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	bool mayPlace(TileSource&, const TilePos& pos) const override;
	void setPlacedOnFace(TileSource&, const TilePos& pos, Facing::Name face) override;
	void neighborChanged(TileSource& source, const TilePos& pos, TileID tile) override;
	void updateShape(TileSource&, const TilePos& pos) override;
	void attack(const TilePos& pos, Player&) override;
	bool use(const TilePos& pos, Player&) override;
	void onRemove(TileSource&, const TilePos& pos) override;
	int getSignal(TileSource&, const TilePos& pos, Facing::Name face) const override;
	int getDirectSignal(TileSource&, const TilePos& pos, Facing::Name face) const override;
	bool isSignalSource() const override;
	void tick(TileSource&, const TilePos& pos, Random*) override;
	void updateDefaultShape() override;

	int unk_h(TileSource&, const TilePos& pos);
	bool checkCanSurvive(TileSource&, const TilePos& pos);
};
