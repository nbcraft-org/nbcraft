#include "NotGateTile.hpp"
#include "world/level/Level.hpp"

NotGateTile::NotGateTile(int ID, int texture, bool on) : TorchTile(ID, texture, Material::decoration)
{
	m_bOn = on;
	setTicking(true);
}

int NotGateTile::getTexture(Facing::Name face, TileData data) const
{
	return face == Facing::UP ? Tile::redStoneDust->getTexture(face, data) : TorchTile::getTexture(face, data);
}

bool NotGateTile::isToggledTooFrequently(Level* level, const TilePos& pos, bool add)
{
	if (add)
	{
		m_recentToggles.push_back(Toggle(pos, level->getTime()));
	}

	int count = 0;

	for (int var7 = 0; var7 < m_recentToggles.size(); var7++)
	{
		Toggle toggle = m_recentToggles[var7];
		if (toggle.pos == pos)
		{
			count++;
			if (count >= 8)
			{
				return true;
			}
		}
	}

	return false;
}

int NotGateTile::getTickDelay() const
{
	return 2;
}

void NotGateTile::onPlace(Level* level, const TilePos& pos)
{
	if (level->getData(pos) == 0)
	{
		TorchTile::onPlace(level, pos);
	}

	if (m_bOn)
	{
		level->updateNeighborsAt(pos.below(), m_ID);
		level->updateNeighborsAt(pos.above(), m_ID);
		level->updateNeighborsAt(pos.west(), m_ID);
		level->updateNeighborsAt(pos.east(), m_ID);
		level->updateNeighborsAt(pos.north(), m_ID);
		level->updateNeighborsAt(pos.south(), m_ID);
	}
}

void NotGateTile::onRemove(Level* level, const TilePos& pos)
{
	if (m_bOn)
	{
		level->updateNeighborsAt(pos.below(), m_ID);
		level->updateNeighborsAt(pos.above(), m_ID);
		level->updateNeighborsAt(pos.west(), m_ID);
		level->updateNeighborsAt(pos.east(), m_ID);
		level->updateNeighborsAt(pos.north(), m_ID);
		level->updateNeighborsAt(pos.south(), m_ID);
	}
}

int NotGateTile::getSignal(const LevelSource* level, const TilePos& pos, Facing::Name face) const
{
	if (!m_bOn)
	{
		return false;
	}
	else
	{
		TileData data = level->getData(pos);
		if (data == 5 && face == Facing::UP)
		{
			return false;
		}
		else if (data == 3 && face == Facing::SOUTH)
		{
			return false;
		}
		else if (data == 4 && face == Facing::NORTH)
		{
			return false;
		}
		else if (data == 1 && face == Facing::EAST)
		{
			return false;
		}
		else
		{
			return data != 2 || face != Facing::WEST;
		}
	}
}

bool NotGateTile::hasNeighborSignal(const Level* level, const TilePos& pos)
{
	TileData data = level->getData(pos);
	if (data == 5 && level->getSignal(pos.below(), Facing::DOWN))
	{
		return true;
	}
	else if (data == 3 && level->getSignal(pos.north(), Facing::DOWN))
	{
		return true;
	}
	else if (data == 4 && level->getSignal(pos.south(), Facing::SOUTH))
	{
		return true;
	}
	else if (data == 1 && level->getSignal(pos.below(), Facing::WEST))
	{
		return true;
	}
	else
	{
		return data == 2 && level->getSignal(pos.below(), Facing::EAST);
	}
}

void NotGateTile::tick(Level* level, const TilePos& pos, Random* random)
{
	bool neighborSignal = hasNeighborSignal(level, pos);

	while (m_recentToggles.size() > 0 && level->getTime() - m_recentToggles[0].when > 100L)
	{
		m_recentToggles.erase(m_recentToggles.begin());
	}

	if (m_bOn)
	{
		if (neighborSignal)
		{
			level->setTileAndData(pos, Tile::notGate_off->m_ID, level->getData(pos));
			if (isToggledTooFrequently(level, pos, true))
			{
				level->playSound(pos + 0.5f, "random.fizz", 0.5f, 2.6f + (level->m_random.nextFloat() - level->m_random.nextFloat()) * 0.8f);

				for (int var7 = 0; var7 < 5; var7++)
				{
					float var8 = float(pos.x) + random->nextFloat() * 0.6f + 0.2f;
					float var10 = float(pos.y) + random->nextFloat() * 0.6f + 0.2f;
					float var12 = float(pos.z) + random->nextFloat() * 0.6f + 0.2f;
					level->addParticle("smoke", Vec3(var8, var10, var12));
				}
			}
		}
	}
	else if (!neighborSignal && !isToggledTooFrequently(level, pos, false))
	{
		level->setTileAndData(pos, Tile::notGate_on->m_ID, level->getData(pos));
	}
}

void NotGateTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	TorchTile::neighborChanged(level, pos, tile);
	level->addToTickNextTick(pos, m_ID, getTickDelay());
}

int NotGateTile::getDirectSignal(const Level* level, const TilePos& pos, Facing::Name face) const
{
	return face == Facing::DOWN ? getSignal(level, pos, face) : false;
}

int NotGateTile::getResource(TileData data, Random* random) const
{
	return Tile::notGate_on->m_ID;
}

bool NotGateTile::isSignalSource() const
{
	return true;
}

void NotGateTile::animateTick(Level* level, const TilePos& pos, Random* random)
{
	TileData data = level->getData(pos);
	float x = (float(pos.x) + 0.5f) + (random->nextFloat() - 0.5f) * 0.2f; // var7
	float y = (float(pos.y) + 0.7f) + (random->nextFloat() - 0.5f) * 0.2f; // var9
	float z = (float(pos.z) + 0.5f) + (random->nextFloat() - 0.5f) * 0.2f; // var11
	float var13 = 0.22f;
	float var15 = 0.27f;

	switch (data)
	{
	case 1:
		level->addParticle("reddust", Vec3(x - var15, y + var13, z));
		break;
	case 2:
		level->addParticle("reddust", Vec3(x + var15, y + var13, z));
		break;
	case 3:
		level->addParticle("reddust", Vec3(x, y + var13, z - var15));
		break;
	case 4:
		level->addParticle("reddust", Vec3(x, y + var13, z + var15));
		break;
	default:
		level->addParticle("reddust", Vec3(x, y, z));
		break;
	}
}
