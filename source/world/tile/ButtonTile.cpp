#include "ButtonTile.hpp"
#include "world/level/Level.hpp"

ButtonTile::ButtonTile(TileID id, int texture) : Tile(id, texture, Material::decoration)
{
	setTicking(true);
}

AABB* ButtonTile::getAABB(const Level*, const TilePos& pos)
{
	return nullptr;
}

int ButtonTile::getTickDelay() const
{
	return 20;
}

bool ButtonTile::isSolidRender() const
{
	return false;
}

bool ButtonTile::isCubeShaped() const
{
	return false;
}

bool ButtonTile::mayPlace(const Level* level, const TilePos& pos) const
{
	if (level->isSolidTile(pos.west())) return true;
	if (level->isSolidTile(pos.east())) return true;
	if (level->isSolidTile(pos.north())) return true;
	if (level->isSolidTile(pos.south())) return true;

	return false;
}

void ButtonTile::setPlacedOnFace(Level* level, const TilePos& pos, Facing::Name face)
{
	TileData data = level->getData(pos);
	int var7 = data & 8;
	data &= 7;

	/*switch (face)
	{
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
	default:
		data = unk_h(level, pos);
	}*/

	if (face == Facing::NORTH && level->isSolidTile(pos.south()))
		data = 4;
	else if (face == Facing::SOUTH && level->isSolidTile(pos.north()))
		data = 3;
	else if (face == Facing::WEST && level->isSolidTile(pos.east()))
		data = 2;
	else if (face == Facing::EAST && level->isSolidTile(pos.west()))
		data = 1;
	else
		data = unk_h(level, pos);

	level->setData(pos, data + var7);
}

int ButtonTile::unk_h(Level* level, const TilePos& pos)
{
	return level->isSolidTile(pos.west()) ? 1 : (level->isSolidTile(pos.east()) ? 2 : (level->isSolidTile(pos.north()) ? 3 : (level->isSolidTile(pos.south()) ? 4 : 1)));
}

void ButtonTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	if (!checkCanSurvive(level, pos))
		return;

	TileData data = level->getData(pos) & 7;

	bool flag = false;
	if (!level->isSolidTile(pos.west()) && data == 1) flag = true;
	if (!level->isSolidTile(pos.east()) && data == 2) flag = true;
	if (!level->isSolidTile(pos.north()) && data == 3) flag = true;
	if (!level->isSolidTile(pos.south()) && data == 4) flag = true;

	if (!flag)
		return; // all good

	spawnResources(level, pos, level->getData(pos));
	level->setTile(pos, TILE_AIR);
}

bool ButtonTile::checkCanSurvive(Level* level, const TilePos& pos)
{
	if (mayPlace(level, pos))
		return true;

	spawnResources(level, pos, level->getData(pos));
	level->setTile(pos, TILE_AIR);

	return false;
}

void ButtonTile::updateShape(const LevelSource* level, const TilePos& pos)
{
	TileData data = level->getData(pos);
	int var6 = data & 7;
	bool var7 = (data & 8) > 0;
	float var8 = 6.0f / 16.0f;
	float var9 = 10.0f / 16.0f;
	float var10 = 3.0f / 16.0f;
	float var11 = 2.0f / 16.0f;
	if (var7)
	{
		var11 = 1.0f / 16.0f;
	}

	switch (var6)
	{
	case 1:
		setShape(0.0f, var8, 0.5f - var10, var11, var9, 0.5f + var10);
		break;
	case 2:
		setShape(1.0f - var11, var8, 0.5f - var10, 1.0f, var9, 0.5f + var10);
		break;
	case 3:
		setShape(0.5f - var10, var8, 0.0f, 0.5f + var10, var9, var11);
		break;
	case 4:
		setShape(0.5f - var10, var8, 1.0f - var11, 0.5f + var10, var9, 1.0f);
		break;
	}
}

void ButtonTile::attack(Level* level, const TilePos& pos, Player* player)
{
	use(level, pos, player);
}

bool ButtonTile::use(Level* level, const TilePos& pos, Player* player)
{
	TileData data = level->getData(pos);
	int var7 = data & 7;
	int var8 = 8 - (data & 8);
	if (var8 == 0)
	{
		return true;
	}
	else
	{
		level->setData(pos, var7 + var8);
		level->setTilesDirty(pos, pos);
		level->playSound(pos + 0.5f, "random.click", 0.3f, 0.6f);
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

		level->addToTickNextTick(pos, m_ID, getTickDelay());
		return true;
	}
}

void ButtonTile::onRemove(Level* level, const TilePos& pos)
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

int ButtonTile::getSignal(const LevelSource* level, const TilePos& pos, Facing::Name face) const
{
	return (level->getData(pos) & 8) > 0;
}

int ButtonTile::getDirectSignal(const Level* level, const TilePos& pos, Facing::Name face) const
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

bool ButtonTile::isSignalSource() const
{
	return true;
}

void ButtonTile::tick(Level* level, const TilePos& pos, Random* random)
{
	if (!level->m_bIsClientSide)
	{
		int var6 = level->getData(pos);
		if ((var6 & 8) != 0)
		{
			level->setData(pos, var6 & 7);
			level->updateNeighborsAt(pos, m_ID);
			int var7 = var6 & 7;
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

			level->playSound(pos + 0.5f, "random.click", 0.3f, 0.5f);
			level->setTilesDirty(pos, pos);
		}
	}
}

void ButtonTile::updateDefaultShape()
{
	float var1 = 3.0f / 16.0f;
	float var2 = 2.0f / 16.0f;
	float var3 = 2.0f / 16.0f;
	setShape(0.5f - var1, 0.5f - var2, 0.5f - var3, 0.5f + var1, 0.5f + var2, 0.5f + var3);
}