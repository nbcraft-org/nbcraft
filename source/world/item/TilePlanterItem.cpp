/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "TilePlanterItem.hpp"
#include "world/level/Level.hpp"
#include "world/tile/Tile.hpp"
#include "world/level/TileSource.hpp"

TilePlanterItem::TilePlanterItem(int id, int place) : Item(id)
{
	m_tile = Tile::tiles[place]->m_ID;
}

bool TilePlanterItem::useOn(ItemStack* instance, Player* player, const TilePos& pos, Facing::Name face) const
{
	Level& level = player->getLevel();
	TileSource& source = player->getTileSource();

	TilePos tp(pos);

	if (source.getTile(pos) == Tile::topSnow->m_ID)
	{
		face = Facing::DOWN;
	}
	else switch (face)
	{
		case Facing::DOWN: tp.y--; break;
		case Facing::UP: tp.y++; break;
		case Facing::NORTH: tp.z--; break;
		case Facing::SOUTH: tp.z++; break;
		case Facing::WEST: tp.x--; break;
		case Facing::EAST: tp.x++; break;
		default: assert(false); return false; break;
	}

	if (!instance->m_count)
		return false;

	if (!source.mayPlace(m_tile, tp, face, player))
		return true;

	if (!source.setTile(tp, m_tile))
		return true;

	Tile* pTile = Tile::tiles[m_tile];
	pTile->setPlacedOnFace(source, tp, face);
	pTile->setPlacedBy(source, tp, player);
	level.playSound(tp + 0.5f, "step." + pTile->m_pSound->name, (pTile->m_pSound->volume + 1) / 2, (pTile->m_pSound->pitch * 0.8f));

	instance->m_count--;
	return true;
}
