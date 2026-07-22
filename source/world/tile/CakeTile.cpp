#include "CakeTile.hpp"
#include "world/entity/Player.hpp"
#include "world/item/Item.hpp"
#include "world/level/TileSource.hpp"

CakeTile::CakeTile(int id, int texture) : Tile(id, texture, Material::cake)
{
	setTicking(true);
}

void CakeTile::updateShape(const TileSource& source, const TilePos& pos)
{
	TileData i = source.getData(pos);
	constexpr float f = 0.0625f;
	float g = (1 + i * 2) / 16.0f;
	constexpr float h = 0.5f;
	setShape(g, 0.0f, f, 1.0f - f, h, 1.0f - f);
}

void CakeTile::updateDefaultShape()
{
	constexpr float f = 0.0625f;
	constexpr float g = 0.5f;
	setShape(f, 0.0f, f, 1.0f - f, g, 1.0f - f);
}

AABB CakeTile::getTileAABB(TileSource& source, const TilePos& pos)
{
	TileData i = source.getData(pos);
	constexpr float f = 0.0625f;
	float g = (1 + i * 2) / 16.0f;
	constexpr float h = 0.5f;
	m_aabbReturned = AABB(pos.x + g, pos.y, pos.z + f, pos.x + 1 - f, pos.y + h, pos.z + 1 - f);
	return m_aabbReturned;
}

AABB* CakeTile::getAABB(const TileSource& source, const TilePos& pos)
{
	TileData i = source.getData(pos);
	constexpr float f = 0.0625f;
	float g = (1 + i * 2) / 16.0f;
	constexpr float h = 0.5f;
	m_aabbReturned = AABB(pos.x + g, pos.y, pos.z + f, pos.x + 1 - f, pos.y + h, pos.z + 1 - f);
	return &m_aabbReturned;
}

int CakeTile::getTexture(Facing::Name face, TileData data) const
{
	switch (face)
	{
		case (Facing::UP): return m_TextureFrame; break;
		case (Facing::DOWN): return m_TextureFrame + 3; break;
		default: return data > 0 && face == (Facing::WEST) ? m_TextureFrame + 2 : m_TextureFrame + 1;
	}
}

int CakeTile::getTexture(Facing::Name face) const
{
	switch (face)
	{
		case (Facing::UP): return m_TextureFrame; break;
		default: return face == (Facing::DOWN) ? m_TextureFrame + 3 : m_TextureFrame + 1;
	}
}

bool CakeTile::use(const TilePos& pos, Player& player)
{
	eat(pos, player);
	return true;
}

void CakeTile::eat(const TilePos& pos, Player& player)
{
	if (player.m_health < 20)
	{
		TileSource& source = player.getTileSource();

		player.heal(3);
		int i = source.getData(pos) + 1;
		if (i >= 6)
		{
			source.setTile(pos, 0);
		}
		else
		{
			source.setTileAndData(pos, FullTile(this, i));
			source.updateNeighborsAt(pos, m_ID);
		}
	}
}

bool CakeTile::mayPlace(const TileSource& source, const TilePos& pos) const
{
	return !Tile::mayPlace(source, pos) ? false : canSurvive(source, pos);
}

void CakeTile::neighborChanged(TileSource& source, const TilePos& pos, TileID tile)
{
	if (!canSurvive(source, pos)) 
	{
		spawnResources(source, pos, source.getData(pos));
		source.setTile(pos, TILE_AIR);
	}
}

bool CakeTile::canSurvive(const TileSource& source, const TilePos& pos) const
{
	return source.getMaterial(pos.below())->isSolid();
}
