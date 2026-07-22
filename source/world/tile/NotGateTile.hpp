#pragma once

#include "TorchTile.hpp"

class NotGateTile : public TorchTile
{
private:
	struct Toggle {
		TilePos pos;
		int32_t when;

		Toggle(const TilePos& p, int32_t w) : pos(p), when(w) {}
	};

	static std::vector<Toggle> recentToggles;

public:
	NotGateTile(int ID, int texture, bool on);

	int getTexture(Facing::Name face, TileData data) const override;
	int getTickDelay() const override;
	void onPlace(TileSource&, const TilePos& pos) override;
	void onRemove(TileSource&, const TilePos& pos) override;
	int getSignal(const TileSource&, const TilePos& pos, Facing::Name face) const override;
	void tick(TileSource&, const TilePos& pos, Random*) override;
	void neighborChanged(TileSource&, const TilePos& pos, TileID tile) override;
	int getDirectSignal(const TileSource&, const TilePos& pos, Facing::Name face) const override;
	int getResource(TileData data, Random* random) const override;
	bool isSignalSource() const override;
	void animateTick(TileSource&, const TilePos& pos, Random*) override;

private:
	bool isToggledTooFrequently(TileSource& source, const TilePos& pos, bool add);
	bool hasNeighborSignal(TileSource& source, const TilePos& pos);
	bool m_bOn;

};
