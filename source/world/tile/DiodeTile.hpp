#pragma once

#include "Tile.hpp"

class DiodeTile : public Tile
{
public:
	static const constexpr float unk_a[4] = { -0.0625f, 1.0f / 16.0f, 0.1875f, 0.3125f };

	DiodeTile(TileID id, bool on);

	bool isCubeShaped() const override;
	bool mayPlace(const Level*, const TilePos& pos) const override;
	bool canSurvive(const Level*, const TilePos& pos) const override;
	void tick(Level*, const TilePos& pos, Random*) override;
	int getTexture(Facing::Name face, TileData data) const override;
	bool shouldRenderFace(const LevelSource*, const TilePos& pos, Facing::Name face) const override;
	eRenderShape getRenderShape() const override;
	int getTexture(Facing::Name face) const override;
	int getDirectSignal(const Level*, const TilePos& pos, Facing::Name face) const override;
	int getSignal(const LevelSource*, const TilePos& pos, Facing::Name face) const override;
	void neighborChanged(Level* level, const TilePos& pos, TileID tile) override;
	bool use(Level*, const TilePos& pos, Player*) override;
	bool isSignalSource() const override;
	void setPlacedBy(Level*, const TilePos& pos, Mob*) override;
	void onPlace(Level*, const TilePos& pos) override;
	bool isSolidRender() const override;
	int getResource(TileData data, Random* random) const override;
	void animateTick(Level*, const TilePos& pos, Random*) override;

private:
	static const constexpr int unk_b[4] = { 1, 2, 3, 4 };
	bool m_bOn;
	bool shouldTurnOn(Level* level, const TilePos& pos, TileData data);
};