/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
#include "RocketLauncherTile.hpp"
#include "world/level/Level.hpp"
#include "world/level/TileSource.hpp"
#include "world/level/TileTickingQueue.hpp"
#include "world/entity/Rocket.hpp"

#define STATE_RECHARGING (0x1)
#define STATE_POWERED    (0x2)

RocketLauncherTile::RocketLauncherTile(TileID id) : Tile(id, 16*14+2, Material::wood)
{
	m_renderLayer = RENDER_LAYER_ALPHATEST;
	setTicking(true);
}

int RocketLauncherTile::getTexture(Facing::Name face, TileData data) const
{
	return (data & STATE_RECHARGING) ? 16*14+3 : 16*14+2;
}

AABB* RocketLauncherTile::getAABB(TileSource*, const TilePos& pos)
{
	return nullptr;
}

eRenderShape RocketLauncherTile::getRenderShape() const
{
	return SHAPE_CROSS;
}

bool RocketLauncherTile::isCubeShaped() const
{
	return false;
}

bool RocketLauncherTile::isSolidRender() const
{
	return false;
}

bool RocketLauncherTile::use(const TilePos& pos, Player* player)
{
	TileSource& source = player->getTileSource();

	int data = source.getData(pos);
	if (data & STATE_RECHARGING)
		return true;

	source.setTileAndData(pos, FullTile(m_ID, data | STATE_RECHARGING));

	// spawn a rocket
	Level& level = player->getLevel();
	level.addEntity(std::make_unique<Rocket>(source, Vec3(pos) + 0.5f));

	// add a tick so that the rocket launcher will reset
	source.getTickQueue(pos)->add(&source, m_ID, getTickDelay());

	return true;
}

void RocketLauncherTile::neighborChanged(Level* level, const TilePos& pos, TileID newTile)
{
	if (newTile <= 0 || !Tile::tiles[newTile]->isSignalSource())
		return;

	int data = level->getData(pos);

	if (level->hasNeighborSignal(pos))
	{
		if (data & (STATE_POWERED | STATE_RECHARGING))
			return;

		level->setDataNoUpdate(pos, data | STATE_POWERED);
		use(level, pos, nullptr);
	}
	else
	{
		if (data & STATE_POWERED)
			level->setDataNoUpdate(pos, data & ~STATE_POWERED);
	}
}

void RocketLauncherTile::tick(TileSource* source, const TilePos& pos, Random* random)
{
	int data = source.getData(pos);
	if (~data & STATE_RECHARGING)
		return;

	level->setData(pos, data & ~STATE_RECHARGING);
}

int RocketLauncherTile::getTickDelay() const
{
	return 10;
}
