#pragma once

#include "Tile.hpp"

class DiodeTile : public Tile
{
public:
	//static const constexpr float unk_a[4] = { -0.0625f, 1.0f / 16.0f, 0.1875f, 0.3125f };
	static const float unk_a[4];

	DiodeTile(TileID id, bool on);

	bool isCubeShaped() const override;
	bool mayPlace(TileSource&, const TilePos& pos) const override;
	bool canSurvive(TileSource&, const TilePos& pos) const override;
	void tick(TileSource&, const TilePos& pos, Random*) override;
	int getTexture(Facing::Name face, TileData data) const override;
	bool shouldRenderFace(TileSource&, const TilePos& pos, Facing::Name face) const override;
	eRenderShape getRenderShape() const override;
	int getTexture(Facing::Name face) const override;
	int getDirectSignal(TileSource&, const TilePos& pos, Facing::Name face) const override;
	int getSignal(TileSource&, const TilePos& pos, Facing::Name face) const override;
	void neighborChanged(TileSource&, const TilePos& pos, TileID tile) override;
	bool use(const TilePos& pos, Player&) override;
	bool isSignalSource() const override;
	void setPlacedBy(const TilePos& pos, Mob&) override;
	void onPlace(TileSource&, const TilePos& pos) override;
	bool isSolidRender() const override;
	int getResource(TileData data, Random* random) const override;
	void animateTick(TileSource&, const TilePos& pos, Random*) override;

private:
	//static const constexpr int unk_b[4] = { 1, 2, 3, 4 };
	static const int unk_b[4];
	bool m_bOn;
	bool shouldTurnOn(TileSource& source, const TilePos& pos, TileData data);
};
