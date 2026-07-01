#include "RedStoneItem.hpp"
#include "world/entity/Player.hpp"
#include "world/level/TileSource.hpp"
#include "world/tile/Tile.hpp"

RedStoneItem::RedStoneItem(int id) : Item(id)
{
}

bool RedStoneItem::useOn(ItemStack* instance, Player* player, const TilePos& pos, Facing::Name face) const
{
	TileSource& tileSource = player->getTileSource();
	TilePos tp(pos);

	switch (face)
	{
		case Facing::DOWN: tp.y--; break;
		case Facing::UP: tp.y++; break;
		case Facing::NORTH: tp.z--; break;
		case Facing::SOUTH: tp.z++; break;
		case Facing::WEST: tp.x--; break;
		case Facing::EAST: tp.x++; break;
	}

	if (!tileSource.isEmptyTile(tp))
		return false;

	if (Tile::redStoneDust->mayPlace(tileSource, tp))
	{
		instance->m_count--;
		tileSource.setTile(tp, Tile::redStoneDust->m_ID);
	}

	return true;
}
