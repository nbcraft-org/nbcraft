#include "NotGateTile.hpp"
#include "world/level/TileSource.hpp"
#include "world/level/Level.hpp"

std::vector<NotGateTile::Toggle> NotGateTile::recentToggles;

NotGateTile::NotGateTile(int ID, int texture, bool on) : TorchTile(ID, texture, Material::decoration)
{
	m_bOn = on;
	setTicking(true);
}

int NotGateTile::getTexture(Facing::Name face, TileData data) const
{
	return face == Facing::UP ? Tile::redStoneDust->getTexture(face, data) : TorchTile::getTexture(face, data);
}

bool NotGateTile::isToggledTooFrequently(TileSource& source, const TilePos& pos, bool add)
{
	if (add)
	{
		recentToggles.push_back(Toggle(pos, source.getLevel().getTime()));
	}

	int count = 0;

	for (size_t var7 = 0; var7 < recentToggles.size(); var7++)
	{
		Toggle toggle = recentToggles.at(var7);
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

void NotGateTile::onPlace(TileSource& source, const TilePos& pos)
{
	if (source.getData(pos) == 0)
	{
		TorchTile::onPlace(source, pos);
	}

	if (m_bOn)
	{
		source.updateNeighborsAt(pos.below(), m_ID);
		source.updateNeighborsAt(pos.above(), m_ID);
		source.updateNeighborsAt(pos.west(), m_ID);
		source.updateNeighborsAt(pos.east(), m_ID);
		source.updateNeighborsAt(pos.north(), m_ID);
		source.updateNeighborsAt(pos.south(), m_ID);
	}
}

void NotGateTile::onRemove(TileSource& source, const TilePos& pos)
{
	if (m_bOn)
	{
		source.updateNeighborsAt(pos.below(), m_ID);
		source.updateNeighborsAt(pos.above(), m_ID);
		source.updateNeighborsAt(pos.west(), m_ID);
		source.updateNeighborsAt(pos.east(), m_ID);
		source.updateNeighborsAt(pos.north(), m_ID);
		source.updateNeighborsAt(pos.south(), m_ID);
	}
}

int NotGateTile::getSignal(TileSource& source, const TilePos& pos, Facing::Name face) const
{
	if (!m_bOn)
		return false;

	TileData data = source.getData(pos);
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

bool NotGateTile::hasNeighborSignal(TileSource& source, const TilePos& pos)
{
	TileData data = source.getData(pos);
	if (data == 5 && source.getSignal(pos.below(), Facing::DOWN))
	{
		return true;
	}
	else if (data == 3 && source.getSignal(pos.north(), Facing::NORTH))
	{
		return true;
	}
	else if (data == 4 && source.getSignal(pos.south(), Facing::SOUTH))
	{
		return true;
	}
	else if (data == 1 && source.getSignal(pos.west(), Facing::WEST))
	{
		return true;
	}
	else
	{
		return data == 2 && source.getSignal(pos.east(), Facing::EAST);
	}
}

void NotGateTile::tick(TileSource& source, const TilePos& pos, Random* random)
{
	Level& level = source.getLevel();
	bool neighborSignal = hasNeighborSignal(source, pos);

	while (recentToggles.size() > 0 && level.getTime() - recentToggles.at(0).when > 100)
	{
		recentToggles.erase(recentToggles.begin());
	}

	if (m_bOn)
	{
		if (neighborSignal)
		{
			source.setTileAndData(pos, FullTile(Tile::notGate_off, source.getData(pos)));
			if (isToggledTooFrequently(source, pos, true))
			{
				level.playSound(pos + 0.5f, "random.fizz", 0.5f, 2.6f + (level.m_random.nextFloat() - level.m_random.nextFloat()) * 0.8f);

				for (int var7 = 0; var7 < 5; var7++)
				{
					float var8 = float(pos.x) + random->nextFloat() * 0.6f + 0.2f;
					float var10 = float(pos.y) + random->nextFloat() * 0.6f + 0.2f;
					float var12 = float(pos.z) + random->nextFloat() * 0.6f + 0.2f;
					level.addParticle("smoke", Vec3(var8, var10, var12));
				}
			}
		}
	}
	else if (!neighborSignal && !isToggledTooFrequently(source, pos, false))
	{
		source.setTileAndData(pos, FullTile(Tile::notGate_on, source.getData(pos)));
	}
}

void NotGateTile::neighborChanged(TileSource& source, const TilePos& pos, TileID tile)
{
	TorchTile::neighborChanged(source, pos, tile);
	TileTickingQueue* pQueue = source.getTickQueue(pos);
	pQueue->add(source, pos, m_ID, getTickDelay());
}

int NotGateTile::getDirectSignal(TileSource& source, const TilePos& pos, Facing::Name face) const
{
	return face == Facing::DOWN ? getSignal(source, pos, face) : false;
}

int NotGateTile::getResource(TileData data, Random* random) const
{
	return Tile::notGate_on->m_ID;
}

bool NotGateTile::isSignalSource() const
{
	return true;
}

void NotGateTile::animateTick(TileSource& source, const TilePos& pos, Random* random)
{
	if (!m_bOn)
		return;

	Level& level = source.getLevel();

	TileData data = source.getData(pos);
	float x = (float(pos.x) + 0.5f) + (random->nextFloat() - 0.5f) * 0.2f; // var7
	float y = (float(pos.y) + 0.7f) + (random->nextFloat() - 0.5f) * 0.2f; // var9
	float z = (float(pos.z) + 0.5f) + (random->nextFloat() - 0.5f) * 0.2f; // var11
	constexpr float var13 = 0.22f;
	constexpr float var15 = 0.27f;

	switch (data)
	{
	case 1:
		level.addParticle("reddust", Vec3(x - var15, y + var13, z));
		break;
	case 2:
		level.addParticle("reddust", Vec3(x + var15, y + var13, z));
		break;
	case 3:
		level.addParticle("reddust", Vec3(x, y + var13, z - var15));
		break;
	case 4:
		level.addParticle("reddust", Vec3(x, y + var13, z + var15));
		break;
	default:
		level.addParticle("reddust", Vec3(x, y, z));
		break;
	}
}
