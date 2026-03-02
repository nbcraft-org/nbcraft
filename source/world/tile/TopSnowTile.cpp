/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "TopSnowTile.hpp"
#include "world/level/Level.hpp"

TopSnowTile::TopSnowTile(TileID id, int b, Material* c) : Tile(id, b, c)
{
	setShape(0, 0, 0, 1, 0.125f, 1);
	setTicking(true);
}

AABB* TopSnowTile::getAABB(const Level*, const TilePos& pos)
{
	return nullptr;
}

bool TopSnowTile::isCubeShaped() const
{
	return false;
}

bool TopSnowTile::isSolidRender() const
{
	return false;
}

int TopSnowTile::getResource(TileData data, Random* random) const
{
	return Item::snowBall->m_itemID;
}

int TopSnowTile::getResourceCount(Random* random) const
{
	return 0;
}

bool TopSnowTile::mayPlace(const Level* level, const TilePos& pos) const
{
	TileID tile = level->getTile(pos.below());

	if (!tile || !Tile::tiles[tile]->isSolidRender())
		return false;

	return level->getMaterial(pos.below())->blocksMotion();
}

bool TopSnowTile::checkCanSurvive(Level* level, const TilePos& pos)
{
	if (mayPlace(level, pos))
		return true;

	spawnResources(level, pos, level->getData(pos));
	level->setTile(pos, TILE_AIR);
	return false;
}

void TopSnowTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	checkCanSurvive(level, pos);
}

bool TopSnowTile::shouldRenderFace(const LevelSource* level, const TilePos& pos, Facing::Name face) const
{
	if (face == Facing::UP)
		return true;

	if (level->getMaterial(pos) == m_pMaterial)
		return false;

	return Tile::shouldRenderFace(level, pos, face);
}

void TopSnowTile::tick(Level* level, const TilePos& pos, Random* random)
{
	if (level->getBrightness(LightLayer::Block, pos) > 11)
	{
		spawnResources(level, pos, level->getData(pos));
		level->setTile(pos, TILE_AIR);
	}
}

void TopSnowTile::playerDestroy(Level* level, Player* player, const TilePos& pos, TileData data)
{
	int var6 = Item::snowBall->m_itemID;
	float var7 = 0.7f;
	double var8 = (level->m_random.nextFloat() * var7) + (1.0f - var7) * 0.5f;
	double var10 = (level->m_random.nextFloat() * var7) + (1.0f - var7) * 0.5f;
	double var12 = (level->m_random.nextFloat() * var7) + (1.0f - var7) * 0.5f;
	ItemEntity* var14 = new ItemEntity(level, Vec3(pos.x + var8, pos.y + var10, pos.z + var12), ItemStack(var6, 1, 0));
	var14->m_throwTime = 10;
	level->addEntity(var14);
	level->setTile(pos, TILE_AIR);
}
