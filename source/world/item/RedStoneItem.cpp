/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

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

	if (!level->isEmptyTile(tp)) {
		return false;
	}
	else {
		if (Tile::redStoneDust->mayPlace(level, tp)) {
			instance->m_count--;
			level->setTile(tp, Tile::redStoneDust->m_ID);
		}

		return true;
	}
}
