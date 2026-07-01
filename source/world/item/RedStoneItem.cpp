#include "RedStoneItem.hpp"
#include "world/entity/Player.hpp"
#include "world/level/TileSource.hpp"
#include "world/tile/Tile.hpp"

RedStoneItem::RedStoneItem(int id) : Item(id)
{
}

bool RedStoneItem::useOn(ItemStack& itemStack, Player& player, const TilePos& pos, Facing::Name face) const
{
	TileSource& tileSource = player.getTileSource();
	TilePos tp(pos);

	tp = tp.relative(face);

	if (!tileSource.isEmptyTile(tp))
		return false;

	if (Tile::redStoneDust->mayPlace(tileSource, tp))
	{
		itemStack.shrink();
		tileSource.setTile(tp, Tile::redStoneDust->m_ID);
	}

	return true;
}
