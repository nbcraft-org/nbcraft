/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "TileItem.hpp"
#include "network/RakNetInstance.hpp"
#include "network/packets/PlaceBlockPacket.hpp"
#include "world/level/Level.hpp"
#include "world/tile/Tile.hpp"
#include "world/level/TileSource.hpp"

TileItem::TileItem(int id) : Item(id)
{
	id += 256;
	m_tile = id;
	m_icon = Tile::tiles[id]->getTexture(Facing::NORTH);
}

std::string TileItem::getDescriptionId() const
{
	return Tile::tiles[m_tile]->getDescriptionId();
}

std::string TileItem::getDescriptionId(ItemStack& itemStack) const
{
	return Tile::tiles[m_tile]->getDescriptionId();
}

bool TileItem::useOn(ItemStack& itemStack, Player& player, const TilePos& pos, Facing::Name face) const
{
	TileSource& source = player.getTileSource();
	Level& level = player.getLevel();

	TilePos tp(pos);

	if (source.getTile(pos) == Tile::topSnow->m_ID)
	{
		face = Facing::DOWN;
	}
	else
	{
		tp = tp.relative(face);
	}

	if (itemStack.m_count == 0)
		return false;

	if (!source.mayPlace(m_tile, tp, face, player, false, nullptr))
		return false;

	Tile* pTile = Tile::tiles[m_tile];

	if (!source.setTileAndData(tp, FullTile(m_tile, getLevelDataForAuxValue(itemStack.getAuxValue()))))
		return true;

	pTile->setPlacedOnFace(source, tp, face);
	pTile->setPlacedBy(tp, player);

	level.playSound(
		Vec3(tp) + 0.5f,
		"step." + pTile->m_pSound->name,
		(pTile->m_pSound->volume + 1.0f) * 0.5f,
		pTile->m_pSound->pitch * 0.8f
	);

	if (level.m_pRakNetInstance)
		level.m_pRakNetInstance->send(new PlaceBlockPacket(player.m_EntityID, tp, (TileID)m_tile, face, itemStack.getAuxValue()));

	player.useItem(itemStack);
	return true;
}

int TileItem::getColor(int data) const
{
	//@NOTE: Used by tiles rendered as 2d items
	return Tile::tiles[m_tile]->getColor(Facing::UP, data);
}
