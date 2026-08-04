#include "RedStoneItem.hpp"
#include "world/level/Level.hpp"
#include "world/entity/Player.hpp"
#include "world/tile/Tile.hpp"

RedStoneItem::RedStoneItem(int id) : Item(id)
{
}

bool RedStoneItem::useOn(ItemStack* instance, Player* player, Level* level, const TilePos& pos, Facing::Name face) const
{
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

	if (!level->isEmptyTile(tp))
		return false;

	if (Tile::redStoneDust->mayPlace(level, tp))
	{
		instance->m_count--;
		level->setTile(tp, Tile::redStoneDust->m_ID);
	}

	return true;
}
