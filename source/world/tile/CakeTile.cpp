#include "CakeTile.hpp"
#include "world/level/Level.hpp"
#include "world/item/Item.hpp"

CakeTile::CakeTile(int id, int texture) : Tile(id, texture, Material::cake)
{
	setTicking(true);
}

void CakeTile::updateShape(const LevelSource* level, const TilePos& pos)
{
	int i = level->getData(pos);
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

AABB CakeTile::getTileAABB(const Level* pLevel, const TilePos& pos)
{
	int i = pLevel->getData(pos);
	constexpr float f = 0.0625f;
	float g = (1 + i * 2) / 16.0f;
	constexpr float h = 0.5f;
	m_aabbReturned = AABB(pos.x + g, pos.y, pos.z + f, pos.x + 1 - f, pos.y + h, pos.z + 1 - f);
	return m_aabbReturned;
}

AABB* CakeTile::getAABB(const Level* pLevel, const TilePos& pos)
{
	int i = pLevel->getData(pos);
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

bool CakeTile::use(Level* pLevel, const TilePos& pos, Player* player)
{
	eat(pLevel, pos, player);
	return true;
}

void CakeTile::eat(Level* pLevel, const TilePos& pos, Player* player)
{
	if (player->m_health < 20)
	{
		player->heal(3);
		int i = pLevel->getData(pos) + 1;
		if (i >= 6)
		{
			pLevel->setTile(pos, 0);
		}
		else
		{
			pLevel->setData(pos, i);
			pLevel->updateNeighborsAt(pos, m_ID);
		}
	}
}

bool CakeTile::mayPlace(const Level* level, const TilePos& pos) const
{
	return !Tile::mayPlace(level, pos) ? false : canSurvive(level, pos);
}

void CakeTile::neighborChanged(Level* pLevel, const TilePos& pos, TileID tile)
{
	if (!canSurvive(pLevel, pos)) 
	{
		spawnResources(pLevel, pos, pLevel->getData(pos));
		pLevel->setTile(pos, 0);
	}
}

bool CakeTile::canSurvive(const Level* pLevel, const TilePos& pos) const
{
	return pLevel->getMaterial(pos.below())->isSolid();
}
