#pragma once

#include "Tile.hpp"

class ButtonTile : public Tile
{
public:
	ButtonTile(TileID id, int texture);

	AABB* getAABB(const Level*, const TilePos& pos) override;
	int getTickDelay() const override;
	bool isSolidRender() const override;
	bool isCubeShaped() const override;
	bool mayPlace(const Level*, const TilePos& pos) const override;
	void setPlacedOnFace(Level*, const TilePos& pos, Facing::Name face) override;
	void neighborChanged(Level* level, const TilePos& pos, TileID tile) override;
	void updateShape(const LevelSource*, const TilePos& pos) override;
	void attack(Level*, const TilePos& pos, Player*) override;
	bool use(Level*, const TilePos& pos, Player*) override;
	void onRemove(Level*, const TilePos& pos) override;
	int getSignal(const LevelSource*, const TilePos& pos, Facing::Name face) const override;
	int getDirectSignal(const Level*, const TilePos& pos, Facing::Name face) const override;
	bool isSignalSource() const override;
	void tick(Level*, const TilePos& pos, Random*) override;
	void updateDefaultShape() override;

	int unk_h(Level*, const TilePos& pos);
	bool checkCanSurvive(Level*, const TilePos& pos);
};
