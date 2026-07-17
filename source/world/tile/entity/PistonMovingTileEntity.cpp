#include "PistonMovingTileEntity.hpp"
#include "world/tile/MovingPistonTile.hpp"
#include "world/level/Level.hpp"

PistonMovingTileEntity::PistonMovingTileEntity(TileID tile, int meta, Facing::Name direction, bool extending, bool isSourcePiston) 
	: m_tileId(tile)
	, m_meta(meta)
	, m_direction(direction)
	, m_bExtending(extending)
	, m_bIsSourcePiston(isSourcePiston)
	, m_progress(0)
	, m_progressO(0)
{
	m_pType = TileEntityType::piston;
}

int PistonMovingTileEntity::getTileId() const
{
	return m_tileId;
}

int PistonMovingTileEntity::getData() const
{
	return m_meta;
}

bool PistonMovingTileEntity::isExtending() const
{	
	return m_bExtending;
}

Facing::Name PistonMovingTileEntity::getDirection() const
{
	return m_direction;
}

bool PistonMovingTileEntity::isSourcePiston() const
{
	return m_bIsSourcePiston;
}

float PistonMovingTileEntity::getProgress(float partialTick) const
{
	return Mth::Lerp(m_progressO, m_progress, Mth::Min(1.0f, partialTick));
}

Vec3 PistonMovingTileEntity::getOff(float partialTick) const
{
	Vec3 normal = TilePos::ZERO.relative(m_direction);
	return m_bExtending ? normal * (getProgress(partialTick) - 1.0f) : normal * (1.0f - getProgress(partialTick));
}

void PistonMovingTileEntity::tick()
{
	m_progressO = m_progress;
	if (m_progressO >= 1.0f)
	{
		_moveCollidedEntities(1.0f, 0.25f);
		m_pLevel->removeTileEntity(m_pos);
		setRemoved();
		if (m_pLevel->getTile(m_pos) == Tile::movingPiston->m_ID)
			m_pLevel->setTileAndData(m_pos, m_tileId, m_meta);
	}
	else
	{
		m_progress += 0.5f;
		if (m_progress >= 1.0f)
			m_progress = 1.0f;

		if (m_bExtending)
			_moveCollidedEntities(m_progress, m_progress - m_progressO + 1.0f / 16.0f);
	}
}

void PistonMovingTileEntity::load(const CompoundTag& tag)
{
	TileEntity::load(tag);
	m_tileId = tag.getInt32("blockId");
	m_meta = tag.getInt32("blockData");
	m_direction = (Facing::Name) tag.getInt8("facing");
	m_progressO = m_progress = tag.getFloat("progress");
	m_bExtending = tag.getBoolean("extending");
}

void PistonMovingTileEntity::save(CompoundTag& tag) const
{
	TileEntity::save(tag);
	tag.putInt32("blockId", m_tileId);
	tag.putInt32("blockData", m_meta);
	tag.putInt8("facing", m_direction);
	tag.putFloat("progress", m_progressO);
	tag.putBoolean("extending", m_bExtending);
}

void PistonMovingTileEntity::_moveCollidedEntities(float progress, float force)
{
	if (!m_bExtending)
		progress--;
	else
		progress = 1.0f - progress;

	AABB* aabb = ((MovingPistonTile*)Tile::movingPiston)->moveByPositionAndProgress(m_pLevel, m_pos, m_tileId, progress, m_direction);
	if (aabb)
	{
		EntityVector vec = m_pLevel->getEntities(nullptr, *aabb);
		if (!vec.empty())
		{
			Vec3 move = Vec3(TilePos::ZERO.relative(m_direction)) * force;
			for (EntityVector::iterator it = vec.begin(); it != vec.end(); ++it)
				(*it)->move(move);
		}
	}
}

void PistonMovingTileEntity::finalTick()
{
	if (m_progressO < 1.0f)
	{
		m_progressO = m_progress = 1.0f;
		m_pLevel->removeTileEntity(m_pos);
		setRemoved();
		if (m_pLevel->getTile(m_pos) == Tile::movingPiston->m_ID)
			m_pLevel->setTileAndData(m_pos, m_tileId, m_meta);
	}
}
