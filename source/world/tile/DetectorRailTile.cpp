#include "DetectorRailTile.hpp"

DetectorRailTile::DetectorRailTile(TileID id, int texture) : RailTile(id, texture, true)
{
	setTicking(true);
}

int DetectorRailTile::getTickDelay() const
{
	return 20;
}

int DetectorRailTile::getSignal(const TileSource& source, const TilePos& pos, Facing::Name face) const
{
    return isPowered(source.getData(pos));
}

int DetectorRailTile::getDirectSignal(const TileSource& source, const TilePos& pos, Facing::Name face) const
{
    return !isPowered(source.getData(pos)) ? false : face == Facing::UP;
}

bool DetectorRailTile::isSignalSource() const
{
    return true;
}

void DetectorRailTile::entityInside(TileSource& source, const TilePos& pos, Entity*) const
{
	if (source.getLevelConst().m_bIsClientSide) return;

	TileData data = source.getData(pos);
	if (!isPowered(data))
		_setStateIfMinecartInteractsWithRail(source, pos, data);
}

void DetectorRailTile::tick(TileSource& source, const TilePos& pos, Random* random)
{
	if (source.getLevelConst().m_bIsClientSide) return;

	TileData data = source.getData(pos);
	if (isPowered(data))
		_setStateIfMinecartInteractsWithRail(source, pos, data);
}

void DetectorRailTile::_setStateIfMinecartInteractsWithRail(TileSource& source, const TilePos& pos, TileData data) const
{
	bool powered = isPowered(data);
	bool foundCart = false;
	constexpr float cartSize = 2.0f / 16.0f;
	Entity::Vector minecarts = source.getLevelConst().getEntitiesOfCategory(EntityCategories::MINECART, AABB(pos.x + cartSize, pos.y, pos.z + cartSize, pos.x + 1 - cartSize, pos.y + 0.25, pos.z + 1 - cartSize));
	if (minecarts.size() > 0)
		foundCart = true;

	Level& level = source.getLevel();
	if (foundCart)
	{
		if (!powered)
		{
			level.setData(pos, data | C_POWERED_BIT);
			source.updateNeighborsAt(pos, m_ID);
			source.updateNeighborsAt(pos.below(), m_ID);
			source.fireTilesDirty(pos, pos);
		}

		level.tickAfterDelay(pos, m_ID, getTickDelay());
	}
	else if (powered)
	{
		level.setData(pos, data & 7);
		source.updateNeighborsAt(pos, m_ID);
		source.updateNeighborsAt(pos.below(), m_ID);
		source.fireTilesDirty(pos, pos);
	}
}
