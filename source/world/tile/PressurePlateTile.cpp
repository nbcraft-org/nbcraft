#include "PressurePlateTile.hpp"
#include "world/level/Level.hpp"

PressurePlateTile::PressurePlateTile(TileID id, int texture, Sensitivity sensitivity) : Tile(id, texture, Material::stone)
{
	m_sensitivity = sensitivity;
	setTicking(true);
	float var4 = 1.0f / 16.0f;
	setShape(var4, 0.0f, var4, 1.0f - var4, 0.03125f, 1.0f - var4);
}

int PressurePlateTile::getTickDelay() const
{
	return 20;
}

AABB* PressurePlateTile::getAABB(const Level*, const TilePos& pos)
{
	return nullptr;
}

bool PressurePlateTile::isSolidRender() const
{
	return false;
}

bool PressurePlateTile::isCubeShaped() const
{
	return false;
}

bool PressurePlateTile::mayPlace(const Level* level, const TilePos& pos) const
{
	return level->isSolidTile(pos.below());
}

void PressurePlateTile::onPlace(Level*, const TilePos& pos)
{
}

void PressurePlateTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	bool flag = false;
	if (!level->isSolidTile(pos.below())) flag = true;

	if (!flag)
		return; // all good

	spawnResources(level, pos, level->getData(pos));
	level->setTile(pos, TILE_AIR);
}

void PressurePlateTile::tick(Level* level, const TilePos& pos, Random* random)
{
	if (!level->m_bIsClientSide)
	{
		if (level->getData(pos) != 0)
		{
			checkPressed(level, pos);
		}
	}
}

void PressurePlateTile::entityInside(Level* level, const TilePos& pos, Entity* entity) const
{
	if (!level->m_bIsClientSide)
	{
		if (level->getData(pos) != 1)
		{
			checkPressed(level, pos);
		}
	}
}

void PressurePlateTile::checkPressed(Level* level, const TilePos& pos) const
{
	// copied from the redstonev2 branch because there isn't an equivalent of getEntitiesOfClass yet
	bool var5 = level->getData(pos) == 1;
	bool var6 = false;
	float var7 = 0.125f;
	AABB aabb(float(pos.x) + var7, float(pos.y), float(pos.z) + var7, float(pos.x + 1) - var7, float(pos.y) + 0.25f, float(pos.z + 1) - var7);
	EntityVector var8 = level->getEntities(nullptr, aabb);
	switch (m_sensitivity)
	{
	case everything:
		var6 = !var8.empty();
		break;

	case mobs:
		var6 = false;
		for (size_t i = 0; i < var8.size(); i++)
		{
			if ((var8)[i]->isMob())
			{
				var6 = true;
				break;
			}
		}
		break;

	case players:
		var6 = false;
		for (size_t i = 0; i < var8.size(); i++)
		{
			if ((var8)[i]->isPlayer())
			{
				var6 = true;
				break;
			}
		}
		break;
	}

	if (var6 && !var5)
	{
		level->setData(pos, 1);
		level->updateNeighborsAt(pos, m_ID);
		level->updateNeighborsAt(pos.below(), m_ID);
		level->setTilesDirty(pos, pos);
		level->playSound(Vec3(float(pos.x) + 0.5f, float(pos.y) + 0.1f, float(pos.z) + 0.5f), "random.click", 0.3f, 0.6f);
	}
	if (!var6 && var5)
	{
		level->setData(pos, 0);
		level->updateNeighborsAt(pos, m_ID);
		level->updateNeighborsAt(pos.below(), m_ID);
		level->setTilesDirty(pos, pos);
		level->playSound(Vec3(float(pos.x) + 0.5f, float(pos.y) + 0.1f, float(pos.z) + 0.5f), "random.click", 0.3f, 0.5f);
	}

	if (var6)
	{
		level->addToTickNextTick(pos, m_ID, getTickDelay());
	}
}

void PressurePlateTile::onRemove(Level* level, const TilePos& pos)
{
	TileData data = level->getData(pos);
	if (data > 0)
	{
		level->updateNeighborsAt(pos, m_ID);
		level->updateNeighborsAt(pos.below(), m_ID);
	}

	Tile::onRemove(level, pos);
}

void PressurePlateTile::updateShape(const LevelSource* level, const TilePos& pos)
{
	bool var5 = level->getData(pos) == 1;
	float var6 = 1.0f / 16.0f;
	if (var5) {
		setShape(var6, 0.0f, var6, 1.0f - var6, 0.03125f, 1.0f - var6);
	}
	else {
		setShape(var6, 0.0f, var6, 1.0f - var6, 1.0f / 16.0f, 1.0f - var6);
	}
}

int PressurePlateTile::getSignal(const LevelSource* level, const TilePos& pos, Facing::Name face) const
{
	return level->getData(pos) > 0;
}

int PressurePlateTile::getDirectSignal(const Level* level, const TilePos& pos, Facing::Name face) const
{
	return level->getData(pos) == 0 ? false : face == Facing::UP;
}

bool PressurePlateTile::isSignalSource() const
{
	return true;
}

void PressurePlateTile::updateDefaultShape()
{
	float var1 = 0.5f;
	float var2 = 2.0f / 16.0f;
	float var3 = 0.5f;
	setShape(0.5f - var1, 0.5f - var2, 0.5f - var3, 0.5f + var1, 0.5f + var2, 0.5f + var3);
}