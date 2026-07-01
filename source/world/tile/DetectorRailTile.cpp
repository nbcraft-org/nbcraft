#include "DetectorRailTile.hpp"

DetectorRailTile::DetectorRailTile(TileID id, int texture) : RailTile(id, texture, true)
{
	setTicking(true);
}

int DetectorRailTile::getTickDelay() const
{
	return 20;
}

int DetectorRailTile::getSignal(const LevelSource* level, const TilePos& pos, Facing::Name face) const
{
    return (level->getData(pos) & 8) != 0;
}

int DetectorRailTile::getDirectSignal(const Level* level, const TilePos& pos, Facing::Name face) const
{
    return (level->getData(pos) & 8) == 0 ? false : face == Facing::UP;
}

bool DetectorRailTile::isSignalSource() const
{
    return true;
}

void DetectorRailTile::entityInside(Level* level, const TilePos& pos, Entity*) const
{
	if (level->m_bIsClientSide) return;
	
	int data = level->getData(pos);
	if ((data & 8) == 0)
		setStateIfMinecartInteractsWithRail(level, pos, data);
}

void DetectorRailTile::tick(Level* level, const TilePos& pos, Random* random)
{
	if (level->m_bIsClientSide) return;

	int data = level->getData(pos);
	if ((data & 8) != 0)
		setStateIfMinecartInteractsWithRail(level, pos, data);
}

void DetectorRailTile::setStateIfMinecartInteractsWithRail(Level* level, const TilePos& pos, int data) const
{
	bool var6 = (data & 8) != 0;
	bool var7 = false;
	constexpr float var8 = 2.0F / 16.0F;
	EntityVector var9 = level->getEntitiesOfCategory(EntityCategories::MINECART, AABB(pos.x + var8, pos.y, pos.z + var8, pos.x + 1 - var8, pos.y + 0.25, pos.z + 1 - var8));
	if (var9.size() > 0) {
		var7 = true;
	}

	if (var7 && !var6)
	{
		level->setData(pos, data | 8);
		level->updateNeighborsAt(pos, m_ID);
		level->updateNeighborsAt(pos.below(), m_ID);
		level->setTilesDirty(pos, pos);
	}

	if (!var7 && var6)
	{
		level->setData(pos, data & 7);
		level->updateNeighborsAt(pos, m_ID);
		level->updateNeighborsAt(pos.below(), m_ID);
		level->setTilesDirty(pos, pos);
	}

	if (var7)
		level->addToTickNextTick(pos, m_ID, getTickDelay());
}