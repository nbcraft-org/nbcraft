#include "RecordPlayerTile.hpp"
#include "world/level/Level.hpp"

RecordPlayerTile::RecordPlayerTile(TileID id, int texture) : Tile(id, texture, Material::wood)
{
}

int RecordPlayerTile::getTexture(Facing::Name face) const
{
	return m_TextureFrame + (face == Facing::UP ? Facing::UP : Facing::DOWN);
}

bool RecordPlayerTile::use(Level* level, const TilePos& pos, Player* player)
{
	int var6 = level->getData(pos);
	if (var6 > 0)
	{
		dropRecording(level, pos, var6);
		return true;
	}
	else
	{
		return false;
	}
}

void RecordPlayerTile::dropRecording(Level* level, const TilePos& pos, TileData data)
{
	level->playStreamingMusic(Util::EMPTY_STRING, pos);
	level->setData(pos, 0);
	int var6 = Item::record_01->m_itemID + data - 1;
	float var7 = 0.7f;
	float var8 = (level->m_random.nextFloat() * var7) + (1.0f - var7) * 0.5f;
	float var10 = (level->m_random.nextFloat() * var7) + (1.0f - var7) * 0.2f + 0.6f;
	float var12 = (level->m_random.nextFloat() * var7) + (1.0f - var7) * 0.5f;
	ItemEntity* var14 = new ItemEntity(level, Vec3(pos.x + var8, pos.y + var10, pos.z + var12), ItemStack(var6, 1, 0));
	var14->m_throwTime = 10;
	level->addEntity(var14);
}

void RecordPlayerTile::spawnResources(Level* level, const TilePos& pos, TileData data, float fChance)
{
	if (!level->m_bIsClientSide)
	{
		if (data > 0) {
			dropRecording(level, pos, data);
		}

		Tile::spawnResources(level, pos, data, fChance);
	}
}