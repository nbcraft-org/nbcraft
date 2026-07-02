#include "RecordingItem.hpp"
#include "world/level/Level.hpp"
#include "world/entity/Player.hpp"
#include "world/tile/Tile.hpp"
#include "world/tile/RecordPlayerTile.hpp"

RecordingItem::RecordingItem(int id, const std::string& recording) 
	: Item(id)
	, m_recording(recording)
{
	m_maxStackSize = 1;
}

bool RecordingItem::useOn(ItemStack* instance, Player* player, Level* level, const TilePos& pos, Facing::Name face) const
{
	if (level->getTile(pos) == Tile::recordPlayer->m_ID && level->getData(pos) == 0)
	{
		RecordPlayerTile::playRecord(level, pos, m_itemID);
		level->levelEvent(LevelEvent(LevelEvent::SOUND_PLAY_RECORD, pos, m_itemID));
		instance->m_count--;
		return true;
	}
	else
	{
		return false;
	}
}

const std::string& RecordingItem::getStreamingMusic() const
{
	return m_recording;
}
