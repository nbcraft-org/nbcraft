#include "PistonBaseTile.hpp"
#include "MovingPistonTile.hpp"
#include "PistonHeadTile.hpp"
#include "world/level/Level.hpp"
#include "entity/PistonMovingTileEntity.hpp"

PistonBaseTile::PistonBaseTile(TileID id, int texture, bool sticky) 
	: Tile(id, texture, Material::piston)
	, m_bIsSticky(sticky)
	, m_bUpdating(false)
{
	setSoundType(Tile::SOUND_STONE);
	setDestroyTime(0.5f);
}

int PistonBaseTile::getTexture(Facing::Name side, TileData meta) const
{
	return (side == Facing::UP || getFacing(meta) > 5) ? m_TextureFrame : side == Facing::DOWN ? 109 : 108;
}

int PistonBaseTile::getTexture(const LevelSource* level, const TilePos& pos, Facing::Name side) const
{
	TileData meta = level->getData(pos);
	int facing = getFacing(meta);
	return facing > 5 ? m_TextureFrame : (side == facing ? (!isExtended(meta) && m_aabb.min.x <= 0.0 && m_aabb.min.y <= 0.0 && m_aabb.min.z <= 0.0 && m_aabb.max.x >= 1.0 && m_aabb.max.y >= 1.0 && m_aabb.max.z >= 1.0 ? m_TextureFrame : 110) : (side == Facing::OPPOSITE[facing] ? 109 : 108));
}

eRenderShape PistonBaseTile::getRenderShape() const
{
	return SHAPE_PISTON;
}

void PistonBaseTile::triggerEvent(Level* level, const TileEvent& event)
{
	m_bUpdating = true;
	if (event.b0 == 0)
	{
		if (_moveBlocks(level, event.pos, (Facing::Name) event.b1))
		{
			level->setData(event.pos, event.b1 | 8);
			level->playSound(Vec3(event.pos) + 0.5f, "tile.piston.out", 0.5f, level->m_random.nextFloat() * 0.25f + 0.6f);
		}
	}
	else if (event.b0 == 1)
	{
		TilePos relative = event.pos.relative((Facing::Name)event.b1);
		PistonMovingTileEntity* piston = static_cast<PistonMovingTileEntity*>(level->getTileEntity(relative));
		if (piston)
			piston->finalTick();

		level->setTileAndDataNoUpdate(event.pos, Tile::movingPiston->m_ID, event.b1);
		level->setTileEntity(event.pos, MovingPistonTile::newMovingTileEntity(m_ID, event.b1, (Facing::Name)event.b1, false, true));
		if (m_bIsSticky)
		{
			TilePos tp = event.pos.relative((Facing::Name)event.b1, 2);
			TileID tile = level->getTile(tp);
			int data = level->getData(tp);
			bool flag = false;
			if (tile == Tile::movingPiston->m_ID)
			{
				PistonMovingTileEntity* otherPiston = static_cast<PistonMovingTileEntity*>(level->getTileEntity(tp));
				if (otherPiston && otherPiston->getDirection() == event.b1 && otherPiston->isExtending())
				{
					otherPiston->finalTick();
					tile = otherPiston->getTileId();
					data = otherPiston->getData();
					flag = true;
				}
			}

			if (flag || tile <= 0 || !_isPushable(tile, level, tp, false) || Tile::tiles[tile]->getPistonPushReaction() != PushReaction::NORMAL && tile != Tile::piston->m_ID && tile != Tile::stickyPiston->m_ID) {
				if (!flag)
				{
					m_bUpdating = false;
					level->setTile(relative, TILE_AIR);
					m_bUpdating = true;
				}
			}
			else
			{
				m_bUpdating = false;
				level->setTile(tp, TILE_AIR);
				m_bUpdating = true;
				level->setTileAndDataNoUpdate(relative, Tile::movingPiston->m_ID, data);
				level->setTileEntity(relative, MovingPistonTile::newMovingTileEntity(tile, data, (Facing::Name)event.b1, false, false));
			}
		}
		else
		{
			m_bUpdating = false;
			level->setTile(relative, TILE_AIR);
			m_bUpdating = true;
		}

		level->playSound(Vec3(event.pos) + 0.5f, "tile.piston.in", 0.5f, level->m_random.nextFloat() * 0.15f + 0.6f);
	}

	m_bUpdating = false;
}

void PistonBaseTile::setPlacedBy(Level* level, const TilePos& pos, Mob* mob)
{
	level->setData(pos, _getRotationData(level, pos, (Player*)mob));
	if (!level->m_bIsClientSide)
		_checkIfExtend(level, pos);
}

void PistonBaseTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	if (!level->m_bIsClientSide && !m_bUpdating)
		_checkIfExtend(level, pos);
}

void PistonBaseTile::onPlace(Level* level, const TilePos& pos)
{
	if (!level->m_bIsClientSide && !level->getTileEntity(pos))
		_checkIfExtend(level, pos);
}

void PistonBaseTile::updateShape(const LevelSource* source, const TilePos& pos)
{
	int data = source->getData(pos);
	if (isExtended(data))
	{
		switch (getFacing(data))
		{
		case 0:
			setShape(0.0f, 0.25f, 0.0f, 1.0f, 1.0f, 1.0f);
			break;
		case 1:
			setShape(0.0f, 0.0f, 0.0f, 1.0f, 12.0f / 16.0f, 1.0f);
			break;
		case 2:
			setShape(0.0f, 0.0f, 0.25f, 1.0f, 1.0f, 1.0f);
			break;
		case 3:
			setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 12.0f / 16.0f);
			break;
		case 4:
			setShape(0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
			break;
		case 5:
			setShape(0.0f, 0.0f, 0.0f, 12.0f / 16.0f, 1.0f, 1.0f);
		}
	}
	else
		setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
}

void PistonBaseTile::updateDefaultShape()
{
	setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
}

void PistonBaseTile::addAABBs(const Level* level, const TilePos& pos, const AABB* aabb, std::vector<AABB>& out)
{
	setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	Tile::addAABBs(level, pos, aabb, out);
}

bool PistonBaseTile::isSolidRender() const
{
	return false;
}

bool PistonBaseTile::isCubeShaped() const
{
	return false;
}

int PistonBaseTile::getFaceTexture() const
{
	return m_bIsSticky ? 106 : 107;
}

void PistonBaseTile::_checkIfExtend(Level* level, const TilePos& pos)
{
	int data = level->getData(pos);
	int face = getFacing(data);
	bool signal = _getNeighborSignal(level, pos, (Facing::Name)face);
	if (data != 7)
	{
		if (signal && !isExtended(data))
		{
			if (_canMoveBlocks(level, pos, (Facing::Name)face))
			{
				level->setDataNoUpdate(pos, face | 8);
				level->tileEvent(TileEvent(pos, 0, face));
			}
		}
		else if (!signal && isExtended(data))
		{
			level->setDataNoUpdate(pos, face);
			level->tileEvent(TileEvent(pos, 1, face));
		}

	}
}

bool PistonBaseTile::_getNeighborSignal(Level* level, const TilePos& pos, Facing::Name facing)
{
	for (int i = 0; i < 6; ++i)
	{
		Facing::Name dir = (Facing::Name)i;
		TilePos rel = pos.relative(dir);
		if ((facing != dir && level->getSignal(rel, dir)) || (Facing::isHorizontal(dir) && level->getSignal(rel.above(), dir)))
			return true;
	}

	return level->getSignal(pos, Facing::DOWN) || level->getSignal(pos.above(2), Facing::UP);
}

int PistonBaseTile::_getRotationData(Level* level, const TilePos& pos, Player* player)
{
	if (Mth::abs(player->m_pos.x - pos.x) < 2.0f && Mth::abs(player->m_pos.z - pos.z) < 2.0f)
	{
		float var5 = player->m_pos.y + 0.2;
		if (var5 - pos.y > 2.0)
			return 1;

		if (pos.y - var5 > 0.0)
			return 0;
	}

	int var7 = Mth::floor((player->m_rot.yaw * 4.0f / 360.0f) + 0.5) & 3;
	return var7 == 0 ? 2 : (var7 == 1 ? 5 : (var7 == 2 ? 3 : (var7 == 3 ? 4 : 0)));
}

bool PistonBaseTile::_isPushable(TileID tile, Level* level, const TilePos& pos, bool destroy)
{
	if (tile == Tile::obsidian->m_ID)
		return false;
	else
	{
		if (tile != Tile::piston->m_ID && tile != Tile::stickyPiston->m_ID)
		{
			if (Tile::tiles[tile]->m_hardness == -1.0f)
				return false;

			if (Tile::tiles[tile]->getPistonPushReaction() == PushReaction::BLOCK)
				return false;

			if (!destroy && Tile::tiles[tile]->getPistonPushReaction() == PushReaction::DESTROY)
				return false;
		}
		else if (isExtended(level->getData(pos)))
			return false;

		return level->getTileEntity(pos) == nullptr;
	}
}

bool PistonBaseTile::_canMoveBlocks(Level* level, const TilePos& pos, Facing::Name facing)
{
	TilePos tp = pos.relative(facing);
	int steps = 0;

	while (true)
	{
		if (steps < 13)
		{
			if (tp.y <= 0 || tp.y >= 127)
				return false;

			TileID tile = level->getTile(tp);
			if (tile)
			{
				if (!_isPushable(tile, level, tp, true))
					return false;

				if (Tile::tiles[tile]->getPistonPushReaction() != PushReaction::DESTROY)
				{
					if (steps == 12)
						return false;

					tp = tp.relative(facing);
					++steps;
					continue;
				}
			}
		}

		return true;
	}
}

bool PistonBaseTile::_moveBlocks(Level* level, const TilePos& pos, Facing::Name facing)
{
	TilePos tp = pos.relative(facing);
	int steps = 0;

	while (true)
	{
		if (steps < 13)
		{
			if (tp.y <= 0 || tp.y >= 127)
				return false;

			TileID tile = level->getTile(tp);
			if (tile)
			{
				if (!_isPushable(tile, level, tp, true))
					return false;

				if (Tile::tiles[tile]->getPistonPushReaction() != PushReaction::DESTROY)
				{
					if (steps == 12)
						return false;

					tp = tp.relative(facing);
					++steps;
					continue;
				}

				Tile::tiles[tile]->spawnResources(level, tp, level->getData(tp));
				level->setTile(tp, TILE_AIR);
			}
		}

		while (tp.x != pos.x || tp.y != pos.y || tp.z != pos.z)
		{
			TilePos newTp = tp.relative(Facing::OPPOSITE[facing]);
			TileID tile = level->getTile(newTp);
			int data = level->getData(newTp);
			if (tile == m_ID && newTp.x == pos.x && newTp.y == pos.y && newTp.z == pos.z)
			{
				level->setTileAndDataNoUpdate(tp, Tile::movingPiston->m_ID, facing | (m_bIsSticky ? 8 : 0));
				level->setTileEntity(tp, MovingPistonTile::newMovingTileEntity(Tile::pistonHead->m_ID, facing | (m_bIsSticky ? 8 : 0), facing, true, false));
			}
			else
			{
				level->setTileAndDataNoUpdate(tp, Tile::movingPiston->m_ID, data);
				level->setTileEntity(tp, MovingPistonTile::newMovingTileEntity(tile, data, facing, true, false));
			}
			tp = newTp;
		}

		return true;
	}
}
