#include "DiodeTile.hpp"
#include "world/entity/Player.hpp"
#include "world/level/TileSource.hpp"
#include "world/level/TileTickingQueue.hpp"
#include "world/level/Level.hpp"

const float DiodeTile::unk_a[4] = { -0.0625f, 1.0f / 16.0f, 0.1875f, 0.3125f };
const int DiodeTile::unk_b[4] = { 1, 2, 3, 4 };

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

bool DiodeTile::mayPlace(const TileSource& source, const TilePos& pos) const
{
	return !source.isSolidBlockingTile(pos.below()) ? false : Tile::mayPlace(source, pos);
}

bool DiodeTile::canSurvive(const TileSource& source, const TilePos& pos) const
{
	return !source.isSolidBlockingTile(pos.below()) ? false : Tile::canSurvive(source, pos);
}

void DiodeTile::tick(TileSource& source, const TilePos& pos, Random* random)
{
	TileData data = source.getData(pos);
	bool bShouldTurnOn = shouldTurnOn(source, pos, data);
	if (m_bOn && !bShouldTurnOn)
	{
		source.setTileAndData(pos, FullTile(Tile::diode_off, data));
	}
	else if (!m_bOn)
	{
		source.setTileAndData(pos, FullTile(Tile::diode_on, data));
		if (!bShouldTurnOn)
		{
			int var8 = (data & 12) >> 2;
			TileTickingQueue* pQueue = source.getTickQueue(pos);
			pQueue->add(source, pos, Tile::diode_on->m_ID, unk_b[var8] * 2);
		}
	}
}

int DiodeTile::getTexture(Facing::Name face, TileData data) const
{
	return face == Facing::DOWN ? (m_bOn ? 99 : 115) : (face == Facing::UP ? (m_bOn ? 147 : 131) : 5);
}

bool DiodeTile::shouldRenderFace(TileSource& source, const TilePos& pos, Facing::Name face) const
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

int DiodeTile::getDirectSignal(const TileSource& source, const TilePos& pos, Facing::Name face) const
{
	return getSignal(source, pos, face);
}

int DiodeTile::getSignal(const TileSource& source, const TilePos& pos, Facing::Name face) const
{
	if (!m_bOn)
	{
		return false;
	}
	else
	{
		TileData data = source.getData(pos) & 3;
		return data == 0 && face == Facing::SOUTH ? true : (data == 1 && face == Facing::WEST ? true : (data == 2 && face == Facing::NORTH ? true : data == 3 && face == Facing::EAST));
	}
}

void DiodeTile::neighborChanged(TileSource& source, const TilePos& pos, TileID tile)
{
	if (!canSurvive(source, pos))
	{
		spawnResources(source, pos, source.getData(pos));
		source.setTile(pos, TILE_AIR);
	}
	else
	{
		TileData data = source.getData(pos);
		bool bShouldTurnOn = shouldTurnOn(source, pos, data);
		int var8 = (data & 12) >> 2;
		TileTickingQueue* pQueue = source.getTickQueue(pos);
		if (m_bOn && !bShouldTurnOn)
		{
			pQueue->add(source, pos, m_ID, unk_b[var8] * 2);
		}
		else if (!m_bOn && bShouldTurnOn) 
		{
			pQueue->add(source, pos, m_ID, unk_b[var8] * 2);
		}

	}
}

bool DiodeTile::shouldTurnOn(TileSource& source, const TilePos& pos, TileData data)
{
	int var6 = data & 3;
	switch (var6)
	{
	case 0:
		return source.getSignal(pos.south(), Facing::SOUTH);
	case 1:
		return source.getSignal(pos.west(), Facing::WEST);
	case 2:
		return source.getSignal(pos.north(), Facing::NORTH);
	case 3:
		return source.getSignal(pos.east(), Facing::EAST);
	default:
		return false;
	}
}

bool DiodeTile::use(const TilePos& pos, Player& player)
{
	TileSource& source = player.getTileSource();

	TileData data = source.getData(pos);
	int var7 = (data & 12) >> 2;
	var7 = (var7 + 1) << 2 & 12;
	source.setTileAndData(pos, FullTile(this, var7 | (data & 3)));
	return true;
}

bool DiodeTile::isSignalSource() const
{
	return false;
}

void DiodeTile::setPlacedBy(const TilePos& pos, Mob& mob)
{
	TileSource& source = mob.getTileSource();

	int var6 = ((Mth::floor(float(mob.m_rot.yaw * 4.0f / 360.0f) + 0.5f) & 3) + 2) % 4;
	source.setTileAndData(pos, FullTile(this, var6));

	bool bShouldTurnOn = shouldTurnOn(source, pos, var6);
	if (bShouldTurnOn)
	{
		TileTickingQueue* pQueue = source.getTickQueue(pos);
		pQueue->add(source, pos, m_ID, 1);
	}
}

void DiodeTile::onPlace(TileSource& source, const TilePos& pos)
{
	source.updateNeighborsAt(pos.east(), m_ID);
	source.updateNeighborsAt(pos.west(), m_ID);
	source.updateNeighborsAt(pos.south(), m_ID);
	source.updateNeighborsAt(pos.north(), m_ID);
	source.updateNeighborsAt(pos.below(), m_ID);
	source.updateNeighborsAt(pos.above(), m_ID);
}

bool DiodeTile::isSolidRender() const
{
	return false;
}

int DiodeTile::getResource(TileData data, Random* random) const
{
	return Item::diode->m_itemID;
}

void DiodeTile::animateTick(TileSource& source, const TilePos& pos, Random* random)
{
	Level& level = source.getLevel();

	if (m_bOn)
	{
		TileData data = source.getData(pos);
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

		level.addParticle("reddust", Vec3(x + var13, y, z + var15));
	}
}
