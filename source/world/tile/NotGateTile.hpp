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
	void onPlace(Level*, const TilePos& pos) override;
	void onRemove(Level*, const TilePos& pos) override;
	int getSignal(const LevelSource*, const TilePos& pos, Facing::Name face) const override;
	void tick(Level*, const TilePos& pos, Random*) override;
	void neighborChanged(Level*, const TilePos& pos, TileID tile) override;
	int getDirectSignal(const Level*, const TilePos& pos, Facing::Name face) const override;
	int getResource(TileData data, Random* random) const override;
	bool isSignalSource() const override;
	void animateTick(Level*, const TilePos& pos, Random*) override;

private:
	bool isToggledTooFrequently(Level* level, const TilePos& pos, bool add);
	bool hasNeighborSignal(const Level* level, const TilePos& pos);
	bool m_bOn;

};
