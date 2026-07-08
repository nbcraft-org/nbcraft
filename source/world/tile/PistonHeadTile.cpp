#include "PistonHeadTile.hpp"
#include "world/level/Level.hpp"
#include "PistonBaseTile.hpp"

PistonHeadTile::PistonHeadTile(TileID id, int texture) : Tile(id, texture, Material::piston), m_headTex(-1)
{
	setDestroyTime(0.5f);
}

void PistonHeadTile::onRemove(Level* level, const TilePos& pos)
{
	Tile::onRemove(level, pos);
	int data = level->getData(pos);
	TilePos tp = pos.relative(Facing::OPPOSITE[getDirection(data)]);
	TileID tile = level->getTile(tp);
	if (tile == Tile::piston->m_ID || tile == Tile::stickyPiston->m_ID)
	{
		data = level->getData(tp);
		if (PistonBaseTile::isExtended(data))
		{
			Tile::tiles[tile]->spawnResources(level, tp, data);
			level->setTile(tp, TILE_AIR);
		}
	}
}

int PistonHeadTile::getTexture(Facing::Name side, TileData meta) const
{
	int dir = getDirection(meta);
	return side == dir ? (m_headTex >= 0 ? m_headTex : ((meta & 8) != 0 ? m_TextureFrame - 1 : m_TextureFrame)) : (side == Facing::OPPOSITE[dir] ? 107 : 108);
}

eRenderShape PistonHeadTile::getRenderShape() const
{
	return SHAPE_PISTON_HEAD;
}

void PistonHeadTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	int data = getDirection(level->getData(pos));
	TilePos tp = pos.relative(Facing::OPPOSITE[data]);
	TileID ot = level->getTile(tp);
	if (ot != Tile::piston->m_ID && ot != Tile::stickyPiston->m_ID)
		level->setTile(pos, TILE_AIR);
	else
		Tile::tiles[ot]->neighborChanged(level, tp, tile);
}

void PistonHeadTile::updateShape(const LevelSource* source, const TilePos& pos)
{
	int data = source->getData(pos);
	switch (getDirection(data))
	{
	case Facing::DOWN:
		setShape(0.0f, 0.0f, 0.0f, 1.0f, 0.25f, 1.0f);
		break;
	case Facing::UP:
		setShape(0.0f, 12.0f / 16.0f, 0.0f, 1.0f, 1.0f, 1.0f);
		break;
	case Facing::NORTH:
		setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.25f);
		break;
	case Facing::SOUTH:
		setShape(0.0f, 0.0f, 12.0f / 16.0f, 1.0f, 1.0f, 1.0f);
		break;
	case Facing::WEST:
		setShape(0.0f, 0.0f, 0.0f, 0.25f, 1.0f, 1.0f);
		break;
	case Facing::EAST:
		setShape(12.0f / 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	}

}

void PistonHeadTile::addAABBs(const Level* level, const TilePos& pos, const AABB* aabb, std::vector<AABB>& out)
{
	int var7 = level->getData(pos);
	switch (getDirection(var7)) {
	case 0:
		setShape(0.0f, 0.0f, 0.0f, 1.0f, 0.25f, 1.0f);
		Tile::addAABBs(level, pos, aabb, out);
		setShape(6.0f / 16.0f, 0.25f, 6.0f / 16.0f, 10.0f / 16.0f, 1.0f, 10.0f / 16.0f);
		Tile::addAABBs(level, pos, aabb, out);
		break;
	case 1:
		setShape(0.0f, 12.0f / 16.0f, 0.0f, 1.0f, 1.0f, 1.0f);
		Tile::addAABBs(level, pos, aabb, out);
		setShape(6.0f / 16.0f, 0.0f, 6.0f / 16.0f, 10.0f / 16.0f, 12.0f / 16.0f, 10.0f / 16.0f);
		Tile::addAABBs(level, pos, aabb, out);
		break;
	case 2:
		setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.25f);
		Tile::addAABBs(level, pos, aabb, out);
		setShape(0.25f, 6.0f / 16.0f, 0.25f, 12.0f / 16.0f, 10.0f / 16.0f, 1.0f);
		Tile::addAABBs(level, pos, aabb, out);
		break;
	case 3:
		setShape(0.0f, 0.0f, 12.0f / 16.0f, 1.0f, 1.0f, 1.0f);
		Tile::addAABBs(level, pos, aabb, out);
		setShape(0.25f, 6.0f / 16.0f, 0.0f, 12.0f / 16.0f, 10.0f / 16.0f, 12.0f / 16.0f);
		Tile::addAABBs(level, pos, aabb, out);
		break;
	case 4:
		setShape(0.0f, 0.0f, 0.0f, 0.25f, 1.0f, 1.0f);
		Tile::addAABBs(level, pos, aabb, out);
		setShape(6.0f / 16.0f, 0.25f, 0.25f, 10.0f / 16.0f, 12.0f / 16.0f, 1.0f);
		Tile::addAABBs(level, pos, aabb, out);
		break;
	case 5:
		setShape(12.0f / 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
		Tile::addAABBs(level, pos, aabb, out);
		setShape(0.0f, 6.0f / 16.0f, 0.25f, 12.0f / 16.0f, 10.0f / 16.0f, 12.0f / 16.0f);
		Tile::addAABBs(level, pos, aabb, out);
	}

	setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
}

bool PistonHeadTile::isSolidRender() const
{
	return false;
}

bool PistonHeadTile::isCubeShaped() const
{
	return false;
}

bool PistonHeadTile::mayPlace(const Level*, const TilePos& pos) const
{
	return false;
}

//bool PistonHeadTile::mayPlace(const Level*, const TilePos& pos, Facing::Name face) const
//{
//	return false;
//}

int PistonHeadTile::getResourceCount(Random*) const
{
	return 0;
}

void PistonHeadTile::setHeadTexture(int texture)
{
	m_headTex = texture;
}

void PistonHeadTile::resetHeadTexture()
{
	m_headTex = -1;
}
