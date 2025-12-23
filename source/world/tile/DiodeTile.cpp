#include "DiodeTile.hpp"
#include "world/level/Level.hpp"

DiodeTile::DiodeTile(TileID ID, bool on) : Tile(ID, TEXTURE_PUMPKIN_TOP, Material::decoration) // what the fuck? (this is like this in b1.3)
{
	m_renderLayer = RENDER_LAYER_ALPHATEST;
	m_bOn = on;
	setShape(0.0f, 0.0f, 0.0f, 1.0f, 2.0f / 16.0f, 1.0f);
}

bool DiodeTile::isCubeShaped() const
{
	return false;
}

bool DiodeTile::mayPlace(const Level* level, const TilePos& pos) const
{
	return !level->isSolidTile(pos.below()) ? false : Tile::mayPlace(level, pos);
}

bool DiodeTile::canSurvive(const Level* level, const TilePos& pos) const
{
	return !level->isSolidTile(pos.below()) ? false : Tile::canSurvive(level, pos);
}

void DiodeTile::tick(Level* level, const TilePos& pos, Random* random)
{
	TileData data = level->getData(pos);
	bool bShouldTurnOn = shouldTurnOn(level, pos, data);
	if (m_bOn && !bShouldTurnOn)
	{
		level->setTileAndData(pos, Tile::diode_off->m_ID, data);
	}
	else if (!m_bOn)
	{
		level->setTileAndData(pos, Tile::diode_on->m_ID, data);
		if (!bShouldTurnOn)
		{
			int var8 = (data & 12) >> 2;
			level->addToTickNextTick(pos, Tile::diode_on->m_ID, unk_b[var8] * 2);
		}
	}
}

int DiodeTile::getTexture(Facing::Name face, TileData data) const
{
	return face == Facing::DOWN ? (m_bOn ? 99 : 115) : (face == Facing::UP ? (m_bOn ? 147 : 131) : 5);
}

bool DiodeTile::shouldRenderFace(const LevelSource* level, const TilePos& pos, Facing::Name face) const
{
	return face != Facing::DOWN && face != Facing::UP;
}

eRenderShape DiodeTile::getRenderShape() const
{
	return SHAPE_DIODE;
}

int DiodeTile::getTexture(Facing::Name face) const
{
	return getTexture(face, 0);
}

int DiodeTile::getDirectSignal(const Level* level, const TilePos& pos, Facing::Name face) const
{
	return getSignal(level, pos, face);
}

int DiodeTile::getSignal(const LevelSource* level, const TilePos& pos, Facing::Name face) const
{
	if (!m_bOn)
	{
		return false;
	}
	else
	{
		TileData data = level->getData(pos) & 3;
		return data == 0 && face == Facing::SOUTH ? true : (data == 1 && face == Facing::WEST ? true : (data == 2 && face == Facing::NORTH ? true : data == 3 && face == Facing::EAST));
	}
}

void DiodeTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	if (!canSurvive(level, pos))
	{
		spawnResources(level, pos, level->getData(pos));
		level->setTile(pos, TILE_AIR);
	}
	else
	{
		TileData data = level->getData(pos);
		bool bShouldTurnOn = shouldTurnOn(level, pos, data);
		int var8 = (data & 12) >> 2;
		if (m_bOn && !bShouldTurnOn)
		{
			level->addToTickNextTick(pos, m_ID, unk_b[var8] * 2);
		}
		else if (!m_bOn && bShouldTurnOn) 
		{
			level->addToTickNextTick(pos, m_ID, unk_b[var8] * 2);
		}

	}
}

bool DiodeTile::shouldTurnOn(Level* level, const TilePos& pos, TileData data)
{
	int var6 = data & 3;
	switch (var6)
	{
	case 0:
		return level->getSignal(pos.south(), Facing::SOUTH);
	case 1:
		return level->getSignal(pos.west(), Facing::WEST);
	case 2:
		return level->getSignal(pos.north(), Facing::NORTH);
	case 3:
		return level->getSignal(pos.east(), Facing::EAST);
	default:
		return false;
	}
}

bool DiodeTile::use(Level* level, const TilePos& pos, Player* player)
{
	TileData data = level->getData(pos);
	int var7 = (data & 12) >> 2;
	var7 = var7 + 1 << 2 & 12;
	level->setData(pos, var7 | data & 3);
	return true;
}

bool DiodeTile::isSignalSource() const
{
	return false;
}

void DiodeTile::setPlacedBy(Level* level, const TilePos& pos, Mob* mob)
{
	int var6 = ((Mth::floor(float(mob->m_rot.x * 4.0f / 360.0f) + 0.5f) & 3) + 2) % 4; // m_rot.x is supposed to be m_rot.y but brent inverted it?!
	level->setData(pos, var6);
	bool bShouldTurnOn = shouldTurnOn(level, pos, var6);
	if (bShouldTurnOn) {
		level->addToTickNextTick(pos, m_ID, 1);
	}
}

void DiodeTile::onPlace(Level* level, const TilePos& pos)
{
	level->updateNeighborsAt(pos.east(), m_ID);
	level->updateNeighborsAt(pos.west(), m_ID);
	level->updateNeighborsAt(pos.south(), m_ID);
	level->updateNeighborsAt(pos.north(), m_ID);
	level->updateNeighborsAt(pos.below(), m_ID);
	level->updateNeighborsAt(pos.above(), m_ID);
}

bool DiodeTile::isSolidRender() const
{
	return false;
}

int DiodeTile::getResource(TileData data, Random* random) const
{
	return Item::diode->m_itemID;
}

void DiodeTile::animateTick(Level* level, const TilePos& pos, Random* random)
{
	if (m_bOn)
	{
		TileData data = level->getData(pos);
		float x = (float(pos.x) + 0.5f) + (random->nextFloat() - 0.5f) * 0.2f; // var7
		float y = (float(pos.y) + 0.4f) + (random->nextFloat() - 0.5f) * 0.2f; // var9
		float z = (float(pos.z) + 0.5f) + (random->nextFloat() - 0.5f) * 0.2f; // var11
		float var13 = 0.0f;
		float var15 = 0.0f;
		if (random->nextInt(2) == 0) 
		{
			switch (data & 3)
			{
			case 0:
				var15 = -0.3125f;
				break;
			case 1:
				var13 = 0.3125f;
				break;
			case 2:
				var15 = 0.3125f;
				break;
			case 3:
				var13 = -0.3125f;
			}
		}
		else
		{
			int var17 = (data & 12) >> 2;
			switch (data & 3) {
			case 0:
				var15 = unk_a[var17];
				break;
			case 1:
				var13 = -unk_a[var17];
				break;
			case 2:
				var15 = -unk_a[var17];
				break;
			case 3:
				var13 = unk_a[var17];
			}
		}

		level->addParticle("reddust", Vec3(x + var13, y, z + var15));
	}
}
