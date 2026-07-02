#include "PressurePlateTile.hpp"
#include "world/level/Level.hpp"
#include "world/level/TileSource.hpp"

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

AABB* PressurePlateTile::getAABB(TileSource& source, const TilePos& pos)
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

bool PressurePlateTile::mayPlace(TileSource& source, const TilePos& pos) const
{
	return source.isSolidBlockingTile(pos.below());
}

void PressurePlateTile::onPlace(TileSource& source, const TilePos& pos)
{
}

void PressurePlateTile::neighborChanged(TileSource& source, const TilePos& pos, TileID tile)
{
	if (source.isSolidBlockingTile(pos.below()))
		return; // all good

	spawnResources(source, pos, source.getData(pos));
	source.setTile(pos, TILE_AIR);
}

void PressurePlateTile::tick(TileSource& source, const TilePos& pos, Random* random)
{
	Level& level = source.getLevel();

	if (!level.m_bIsClientSide)
	{
		if (source.getData(pos) != 0)
		{
			checkPressed(source, pos);
		}
	}
}

void PressurePlateTile::entityInside(TileSource& source, const TilePos& pos, Entity* entity) const
{
	Level& level = source.getLevel();

	if (!level.m_bIsClientSide)
	{
		if (source.getData(pos) != 1)
		{
			checkPressed(source, pos);
		}
	}
}

void PressurePlateTile::checkPressed(TileSource& source, const TilePos& pos) const
{
	Level& level = source.getLevel();

	// copied from the redstonev2 branch because there isn't an equivalent of getEntitiesOfClass yet
	bool var5 = source.getData(pos) == 1;
	bool var6 = false;
	float var7 = 0.125f;
	AABB aabb(float(pos.x) + var7, float(pos.y), float(pos.z) + var7, float(pos.x + 1) - var7, float(pos.y) + 0.25f, float(pos.z + 1) - var7);
	Entity::Vector var8 = source.getEntities(nullptr, aabb);
	switch (m_sensitivity)
	{
	case SENSITIVITY_EVERYTHING:
		var6 = !var8.empty();
		break;

	case SENSITIVITY_MOBS:
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

	case SENSITIVITY_PLAYERS:
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
		source.setTileAndData(pos, FullTile(m_ID, 1));
		source.updateNeighborsAt(pos, m_ID);
		source.updateNeighborsAt(pos.below(), m_ID);
		//source.setTilesDirty(pos, pos);
		level.playSound(Vec3(float(pos.x) + 0.5f, float(pos.y) + 0.1f, float(pos.z) + 0.5f), "random.click", 0.3f, 0.6f);
	}
	if (!var6 && var5)
	{
		source.setTileAndData(pos, FullTile(m_ID, 0));
		source.updateNeighborsAt(pos, m_ID);
		source.updateNeighborsAt(pos.below(), m_ID);
		//source.setTilesDirty(pos, pos);
		level.playSound(Vec3(float(pos.x) + 0.5f, float(pos.y) + 0.1f, float(pos.z) + 0.5f), "random.click", 0.3f, 0.5f);
	}

	if (var6)
	{
		TileTickingQueue* pQueue = source.getTickQueue(pos);
		pQueue->add(source, pos, m_ID, getTickDelay());
	}
}

void PressurePlateTile::onRemove(TileSource& source, const TilePos& pos)
{
	TileData data = source.getData(pos);
	if (data > 0)
	{
		source.updateNeighborsAt(pos, m_ID);
		source.updateNeighborsAt(pos.below(), m_ID);
	}

	Tile::onRemove(source, pos);
}

void PressurePlateTile::updateShape(TileSource& source, const TilePos& pos)
{
	bool var5 = source.getData(pos) == 1;
	float var6 = 1.0f / 16.0f;
	if (var5) {
		setShape(var6, 0.0f, var6, 1.0f - var6, 0.03125f, 1.0f - var6);
	}
	else {
		setShape(var6, 0.0f, var6, 1.0f - var6, 1.0f / 16.0f, 1.0f - var6);
	}
}

int PressurePlateTile::getSignal(TileSource& source, const TilePos& pos, Facing::Name face) const
{
	return source.getData(pos) > 0;
}

int PressurePlateTile::getDirectSignal(TileSource& source, const TilePos& pos, Facing::Name face) const
{
	return source.getData(pos) == 0 ? false : face == Facing::UP;
}

bool PressurePlateTile::isSignalSource() const
{
	return true;
}

void PressurePlateTile::updateDefaultShape()
{
	constexpr float var1 = 0.5f;
	constexpr float var2 = 2.0f / 16.0f;
	constexpr float var3 = 0.5f;
	setShape(0.5f - var1, 0.5f - var2, 0.5f - var3, 0.5f + var1, 0.5f + var2, 0.5f + var3);
}
