#include "RecordPlayerTile.hpp"
#include "world/entity/Player.hpp"
#include "world/level/Level.hpp"
#include "world/level/TileSource.hpp"

RecordPlayerTile::RecordPlayerTile(TileID id, int texture) : Tile(id, texture, Material::wood)
{
}

int RecordPlayerTile::getTexture(Facing::Name face) const
{
	return m_TextureFrame + (face == Facing::UP ? Facing::UP : Facing::DOWN);
}

bool RecordPlayerTile::use(const TilePos& pos, Player& player)
{
	TileSource& source = player.getTileSource();

	int var6 = source.getData(pos);
	if (var6 == 0)
		return false;

	dropRecording(source, pos, var6);
	return true;
}

void RecordPlayerTile::dropRecording(TileSource& source, const TilePos& pos, TileData data)
{
	Level& level = source.getLevel();

	level.playStreamingMusic(Util::EMPTY_STRING, pos);
	source.setTileAndData(pos, FullTile(this, 0));
	int var6 = Item::record_01->m_itemID + data - 1;
	constexpr float var7 = 0.7f;
	float var8 = (level.m_random.nextFloat() * var7) + (1.0f - var7) * 0.5f;
	float var10 = (level.m_random.nextFloat() * var7) + (1.0f - var7) * 0.2f + 0.6f;
	float var12 = (level.m_random.nextFloat() * var7) + (1.0f - var7) * 0.5f;
	ItemEntity* var14 = new ItemEntity(source, Vec3(pos.x + var8, pos.y + var10, pos.z + var12), ItemStack(var6, 1, 0));
	var14->m_throwTime = 10;
	level.addEntity(var14);
}

void RecordPlayerTile::spawnResources(TileSource& source, const TilePos& pos, TileData data, float fChance)
{
	Level& level = source.getLevel();

	if (!level.m_bIsClientSide)
	{
		if (data > 0) {
			dropRecording(source, pos, data);
		}

		Tile::spawnResources(source, pos, data, fChance);
	}
}
