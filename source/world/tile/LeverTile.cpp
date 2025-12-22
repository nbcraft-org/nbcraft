#include "LeverTile.hpp"
#include "world/level/Level.hpp"

LeverTile::LeverTile(TileID id, int texture) : Tile(id, texture, Material::decoration)
{
	m_renderLayer = RENDER_LAYER_ALPHATEST;
}

AABB* LeverTile::getAABB(const Level*, const TilePos& pos)
{
	return nullptr;
}

bool LeverTile::isSolidRender() const
{
	return false;
}

bool LeverTile::isCubeShaped() const
{
	return false;
}

eRenderShape LeverTile::getRenderShape() const
{
	return SHAPE_LEVER;
}

bool LeverTile::mayPlace(const Level* level, const TilePos& pos) const
{
	if (level->isSolidTile(pos.west())) return true;
	if (level->isSolidTile(pos.east())) return true;
	if (level->isSolidTile(pos.north())) return true;
	if (level->isSolidTile(pos.south())) return true;
	if (level->isSolidTile(pos.below())) return true;

	return false;
}

void LeverTile::setPlacedOnFace(Level* level, const TilePos& pos, Facing::Name face)
{
	TileData data = level->getData(pos);
	int var7 = data & 8;
	data &= 7;

	switch (face)
	{
	case Facing::UP:
		if (level->isSolidTile(pos.below()))
			data = 5 + level->m_random.nextInt(2);
		break;
	case Facing::NORTH:
		if (level->isSolidTile(pos.south()))
			data = 4;
		break;
	case Facing::SOUTH:
		if (level->isSolidTile(pos.north()))
			data = 3;
		break;
	case Facing::WEST:
		if (level->isSolidTile(pos.east()))
			data = 2;
		break;
	case Facing::EAST:
		if (level->isSolidTile(pos.west()))
			data = 1;
		break;
	}

	level->setData(pos, data + var7);
}

void LeverTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	if (!checkCanSurvive(level, pos))
		return;

	TileData data = level->getData(pos) & 7;

	bool flag = false;
	if (!level->isSolidTile(pos.west()) && data == 1) flag = true;
	if (!level->isSolidTile(pos.east()) && data == 2) flag = true;
	if (!level->isSolidTile(pos.north()) && data == 3) flag = true;
	if (!level->isSolidTile(pos.south()) && data == 4) flag = true;
	if (!level->isSolidTile(pos.below()) && data == 5) flag = true;

	if (!flag)
		return; // all good

	spawnResources(level, pos, level->getData(pos));
	level->setTile(pos, TILE_AIR);
}

bool LeverTile::checkCanSurvive(Level* level, const TilePos& pos)
{
	if (mayPlace(level, pos))
		return true;

	spawnResources(level, pos, level->getData(pos));
	level->setTile(pos, TILE_AIR);

	return false;
}

void LeverTile::updateShape(const LevelSource* level, const TilePos& pos)
{
	TileData data = level->getData(pos) & 7;
	float var6 = 3.0f / 16.0f;

	switch (data)
	{
	case 1:
		setShape(0.0f, 0.2f, 0.5f - var6, var6 * 2.0f, 0.8f, 0.5f + var6);
		break;
	case 2:
		setShape(1.0f - var6 * 2.0f, 0.2f, 0.5f - var6, 1.0f, 0.8f, 0.5f + var6);
		break;
	case 3:
		setShape(0.5f - var6, 0.2f, 0.0f, 0.5f + var6, 0.8f, var6 * 2.0f);
		break;
	case 4:
		setShape(0.5f - var6, 0.2f, 1.0f - var6 * 2.0f, 0.5f + var6, 0.8f, 1.0f);
		break;
	default:
		var6 = 0.25f;
		setShape(0.5f - var6, 0.0f, 0.5f - var6, 0.5f + var6, 0.6f, 0.5f + var6);
		break;
	}
}

void LeverTile::attack(Level* level, const TilePos& pos, Player* player)
{
	use(level, pos, player);
}

bool LeverTile::use(Level* level, const TilePos& pos, Player* player)
{
	if (level->m_bIsClientSide) // reminder to self: level->isOnline
	{
		return true;
	}
	else
	{
		TileData data = level->getData(pos);
		int var7 = data & 7;
		int var8 = 8 - (data & 8);
		level->setData(pos, var7 + var8);
		level->setTilesDirty(pos, pos);
		level->playSound(pos + 0.5f, "random.click", 0.3f, var8 > 0 ? 0.6f : 0.5f);
		level->updateNeighborsAt(pos, m_ID);
		switch (var7)
		{
		case 1:
			level->updateNeighborsAt(pos.west(), m_ID);
			break;
		case 2:
			level->updateNeighborsAt(pos.east(), m_ID);
			break;
		case 3:
			level->updateNeighborsAt(pos.north(), m_ID);
			break;
		case 4:
			level->updateNeighborsAt(pos.south(), m_ID);
			break;
		default:
			level->updateNeighborsAt(pos.below(), m_ID);
			break;
		}

		return true;
	}
}

void LeverTile::onRemove(Level* level, const TilePos& pos)
{
	TileData data = level->getData(pos);
	if ((data & 8) > 0)
	{
		level->updateNeighborsAt(pos, m_ID);
		int var6 = data & 7;
		switch (var6)
		{
		case 1:
			level->updateNeighborsAt(pos.west(), m_ID);
			break;
		case 2:
			level->updateNeighborsAt(pos.east(), m_ID);
			break;
		case 3:
			level->updateNeighborsAt(pos.north(), m_ID);
			break;
		case 4:
			level->updateNeighborsAt(pos.south(), m_ID);
			break;
		default:
			level->updateNeighborsAt(pos.below(), m_ID);
			break;
		}
	}

	Tile::onRemove(level, pos);
}

int LeverTile::getSignal(const LevelSource* level, const TilePos& pos, Facing::Name face) const
{
	return (level->getData(pos) & 8) > 0;
}

int LeverTile::getDirectSignal(const Level* level, const TilePos& pos, Facing::Name face) const
{
	TileData data = level->getData(pos);
	if ((data & 8) == 0) 
	{
		return false;
	}
	else 
	{
		int var7 = data & 7;
		if (var7 == 5 && face == Facing::UP) 
		{
			return true;
		}
		else if (var7 == 4 && face == Facing::NORTH) 
		{
			return true;
		}
		else if (var7 == 3 && face == Facing::SOUTH) 
		{
			return true;
		}
		else 
		{
			return var7 == 2 && face == Facing::WEST ? true : var7 == 1 && face == Facing::EAST;
		}
	}
}

bool LeverTile::isSignalSource() const
{
	return true;
}