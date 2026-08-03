#pragma once

#include "Tile.hpp"

enum PistonEvent
{
	PISTON_EXTEND = 0,
	PISTON_RETRACT = 1,
};

class PistonBaseTile : public Tile
{
public:
	PistonBaseTile(TileID id, int texture, bool sticky);

private:
	void _checkIfExtend(Level*, const TilePos& pos);
	bool _getNeighborSignal(Level*, const TilePos& pos, Facing::Name facing);
	static int _getRotationData(Level* level, const TilePos& pos, Player* player);
	static bool _isPushable(TileID tile, Level* level, const TilePos& pos, bool destroy);
	static bool _canMoveBlocks(Level* level, const TilePos& pos, Facing::Name facing);
	bool _moveBlocks(Level* level, const TilePos& pos, Facing::Name facing);

public:
	int getTexture(Facing::Name side, TileData meta) const override;
	int getTexture(const LevelSource*, const TilePos& pos, Facing::Name face) const override;
	eRenderShape getRenderShape() const override;
	void triggerEvent(Level*, const TileEvent& event) override;
	void setPlacedBy(Level*, const TilePos& pos, Mob*) override;
	void neighborChanged(Level*, const TilePos& pos, TileID tile) override;
	void onPlace(Level*, const TilePos& pos) override;
	void updateShape(const LevelSource*, const TilePos&) override;
	void updateDefaultShape() override;
	void addAABBs(const Level* level, const TilePos& pos, const AABB* aabb, std::vector<AABB>& out) override;

	bool isSolidRender() const override;
	bool isCubeShaped() const override;

	int getFaceTexture() const;

	static Facing::Name getFacing(int data) { return (Facing::Name)(data & 7); }
	static bool isExtended(int data) { return (data & 8) != 0; }

private:
	bool m_bIsSticky;
	bool m_bUpdating;
};
