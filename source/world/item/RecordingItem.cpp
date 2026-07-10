#include "RecordingItem.hpp"
#include "world/entity/Player.hpp"
#include "world/level/Level.hpp"
#include "world/level/TileSource.hpp"
#include "world/tile/Tile.hpp"

RecordingItem::RecordingItem(int id, const std::string& recording) : Item(id), m_recording(recording)
{
	m_maxStackSize = 1;
}

bool RecordingItem::useOn(ItemStack& itemStack, Player& player, const TilePos& pos, Facing::Name face) const
{
	Level& level = player.getLevel();
	TileSource& tileSource = player.getTileSource();

	if (tileSource.getTile(pos) == Tile::recordPlayer->m_ID && tileSource.getData(pos) == 0)
	{
		tileSource.setTileAndData(pos, FullTile(Tile::recordPlayer, m_itemID - Item::record_01->m_itemID + 1));
		level.playStreamingMusic(m_recording, pos);
		itemStack.shrink();
		return true;
	}
	else
	{
		return false;
	}
}
