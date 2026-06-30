#include "RecordingItem.hpp"
#include "world/level/Level.hpp"
#include "world/entity/Player.hpp"
#include "world/tile/Tile.hpp"

RecordingItem::RecordingItem(int id, const std::string& recording) : Item(id), m_recording(recording)
{
	m_maxStackSize = 1;
}

bool RecordingItem::useOn(ItemStack* instance, Player* player, Level* level, const TilePos& pos, Facing::Name face) const
{
	if (level->getTile(pos) == Tile::recordPlayer->m_ID && level->getData(pos) == 0)
	{
		level->setData(pos, m_itemID - Item::record_01->m_itemID + 1);
		level->playStreamingMusic(m_recording, pos);
		instance->m_count--;
		return true;
	}
	else
	{
		return false;
	}
}
