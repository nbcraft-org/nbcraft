/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "RocketItem.hpp"
#include "world/level/Level.hpp"
#include "world/level/TileSource.hpp"
#include "world/entity/Player.hpp"
#include "world/entity/Rocket.hpp"

RocketItem::RocketItem(int id) : Item(id)
{
}

bool RocketItem::useOn(ItemStack& itemStack, Player& player, const TilePos& pos, Facing::Name face) const
{
	TilePos tp(pos);
	TileSource& tileSource = player.getTileSource();
	Level& level = player.getLevel();

	if (tileSource.getTile(pos) == Tile::topSnow->m_ID)
	{
		face = Facing::DOWN;
	}
	else
	{
		tp = tp.relative(face);
	}

	level.addEntity(new Rocket(tileSource, tp + 0.5f));

	itemStack.shrink();
	return true;
}
