#include "RecordPlayerTile.hpp"
#include "world/level/Level.hpp"
#include "entity/RecordPlayerTileEntity.hpp"

RecordPlayerTile::RecordPlayerTile(TileID id, int texture) : EntityTile(id, texture, Material::wood)
{
}

int RecordPlayerTile::getTexture(Facing::Name face) const
{
    return m_TextureFrame + (face == Facing::UP ? 1 : 0);
}

bool RecordPlayerTile::use(Level* level, const TilePos& pos, Player* player)
{
    if (level->m_bIsClientSide || !level->getData(pos))
        return false;
    else
    {
        ejectRecord(level, pos);
        return true;
    }
}

void RecordPlayerTile::spawnResources(Level* level, const TilePos& pos, TileData data, float chance)
{
    if (!level->m_bIsClientSide)
        Tile::spawnResources(level, pos, data, chance);
}

void RecordPlayerTile::onRemove(Level* level, const TilePos& pos)
{
    ejectRecord(level, pos);
    Tile::onRemove(level, pos);
}

void RecordPlayerTile::playRecord(Level* level, const TilePos& pos, int record)
{
    if (!level->m_bIsClientSide)
    {
        RecordPlayerTileEntity* player = (RecordPlayerTileEntity*)level->getTileEntity(pos);
        player->m_record = record;
        player->setChanged();
        level->setData(pos, 1);
    }
}

void RecordPlayerTile::ejectRecord(Level* level, const TilePos& pos)
{
    if (!level->m_bIsClientSide)
    {
        RecordPlayerTileEntity* player = (RecordPlayerTileEntity*)level->getTileEntity(pos);
        int record = player->m_record;
        if (record != 0)
		{
            level->levelEvent(LevelEvent(LevelEvent::SOUND_PLAY_RECORD, pos));
            level->playStreamingMusic(Util::EMPTY_STRING, pos);
            player->m_record = 0;
            player->setChanged();
            level->setData(pos, 0);
            float var8 = 0.7f;
            float var9 = (level->m_random.nextFloat() * var8) + (1.0f - var8) * 0.5f;
            float var11 = (level->m_random.nextFloat() * var8) + (1.0f - var8) * 0.2f + 0.6f;
            float var13 = (level->m_random.nextFloat() * var8) + (1.0f - var8) * 0.5f;
            ItemEntity* var15 = new ItemEntity(level, Vec3(pos.x + var9, pos.y + var11, pos.z + var13), ItemStack(record, 1, 0));
            var15->m_throwTime = 10;
            level->addEntity(var15);
        }
    }
}

TileEntity* RecordPlayerTile::newTileEntity()
{
	return new RecordPlayerTileEntity;
}