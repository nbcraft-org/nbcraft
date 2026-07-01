#include "RailTile.hpp"
#include "Rail.hpp"

RailTile::RailTile(TileID id, int texture, bool isPowered) : Tile(id, texture, Material::decoration), m_bIsPowered(isPowered)
{
	setShape(0.0F, 0.0F, 0.0F, 1.0F, 2.0F / 16.0F, 1.0F);
	m_renderLayer = RENDER_LAYER_ALPHATEST;
}

bool RailTile::mayPlace(const Level* level, const TilePos& pos) const
{
	return level->isSolidBlockingTile(pos.below());
}

void RailTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	if (level->m_bIsClientSide) return;
	
	TileData data = level->getData(pos);
	int faceData = data;

	if (m_bIsPowered)
		faceData = faceData & 7;

	bool var7 = false;

	if (!level->isSolidBlockingTile(pos.below()))
		var7 = true;
	else if (faceData == 2 && !level->isSolidBlockingTile(pos.east()))
		var7 = true;
	else if (faceData == 3 && !level->isSolidBlockingTile(pos.west()))
		var7 = true;
	else if (faceData == 4 && !level->isSolidBlockingTile(pos.north()))
		var7 = true;
	else if (faceData == 5 && !level->isSolidBlockingTile(pos.south()))
		var7 = true;

	if (var7)
	{
		spawnResources(level, pos, level->getData(pos));
		level->setTile(pos, TILE_AIR);
	}
	else if (isPoweredRail(this))
	{
		bool var9 = level->hasNeighborSignal(pos) || level->hasNeighborSignal(pos.above());
		var9 = var9 || applyPower(level, pos, data, true, 0) || applyPower(level, pos, data, false, 0);
		bool var10 = false;
		if (var9 && (data & 8) == 0)
		{
			level->setData(pos, faceData | 8);
			var10 = true;
		}
		else if (!var9 && (data & 8) != 0)
		{
			level->setData(pos, faceData);
			var10 = true;
		}

		if (var10)
		{
			level->updateNeighborsAt(pos.below(), m_ID);
			if (faceData == 2 || faceData == 3 || faceData == 4 || faceData == 5) {
				level->updateNeighborsAt(pos.above(), m_ID);
			}
		}
	}
	else if (tile > 0 && Tile::tiles[tile]->isSignalSource() && !m_bIsPowered && (Rail(level, pos)).countPotentialConnections() == 3) {
		updateDir(level, pos, false);
	}
}

bool RailTile::isSolidRender() const
{
	return false;
}

bool RailTile::isCubeShaped() const
{
	return false;
}

AABB* RailTile::getAABB(const Level*, const TilePos& pos)
{
	return nullptr;
}

eRenderShape RailTile::getRenderShape() const
{
	return SHAPE_RAIL;
}

void RailTile::updateShape(const LevelSource* level, const TilePos& pos)
{
	TileData var5 = level->getData(pos);
	if (var5 >= 2 && var5 <= 5)
		setShape(0.0F, 0.0F, 0.0F, 1.0F, 10.0F / 16.0F, 1.0F);
	else
		setShape(0.0F, 0.0F, 0.0F, 1.0F, 2.0F / 16.0F, 1.0F);
}

void RailTile::onPlace(Level* level, const TilePos& pos)
{
	updateDir(level, pos, true);

	TileData data = level->getData(pos);
	int faceData = data;
	if (m_bIsPowered) {
		faceData = data & 7;
	}

	if (isPoweredRail(this)) {
		bool var9 = level->hasNeighborSignal(pos) || level->hasNeighborSignal(pos.above());
		var9 = var9 || applyPower(level, pos, data, true, 0) || applyPower(level, pos, data, false, 0);
		bool var10 = false;
		if (var9 && (data & 8) == 0)
		{
			level->setData(pos, faceData | 8);
			var10 = true;
		}
		else if (!var9 && (data & 8) != 0)
		{
			level->setData(pos, faceData);
			var10 = true;
		}

		if (var10)
		{
			level->updateNeighborsAt(pos.below(), m_ID);
			if (faceData == 2 || faceData == 3 || faceData == 4 || faceData == 5)
				level->updateNeighborsAt(pos.above(), m_ID);
		}
	}
}

void RailTile::setPlacedBy(Level* level, const TilePos& pos, Mob* mob)
{
	int rot = Mth::floor(0.5f + (mob->m_rot.yaw * 4.0f / 360.0f)) & 3;
	if (rot == 1 || rot == 3)
		level->setData(pos, 1);

	//@NOTE: neighborChanged is called first in the original, but this was causing problems with powered rails being unpowered, certainly it's called for some reason after setPlacedBy
	updateDir(level, pos, true);
	neighborChanged(level, pos, 0);

}

void RailTile::updateDir(Level* level, const TilePos& pos, bool updateNeighbors)
{
	if (!level->m_bIsClientSide) Rail(level, pos).place(level->hasNeighborSignal(pos), updateNeighbors);
}

int RailTile::getTexture(Facing::Name face, TileData data) const
{
	if (m_bIsPowered)
	{
		if (isPoweredRail(this) && (data & 8) != 0)
			return m_TextureFrame + 16;
	}
	else if (data >= 6)
		return m_TextureFrame - 16;

	return m_TextureFrame;
}

bool RailTile::applyPower(Level* level, const TilePos& pos, TileData data, bool var6, int var7) {
	if (var7 >= 8)
		return false;

	TilePos tp(pos);
	int var8 = data & 7;
	bool var9 = true;
	switch (var8) {
	case 0:
	{
		if (var6)
			++tp.z;
		else
			--tp.z;
		break;
	}
	case 1:
	{
		if (var6)
			--tp.x;
		else
			++tp.x;
		break;
	}
	case 2:
	{
		if (var6)
			--tp.x;
		else
		{
			++tp.x;
			++tp.y;
			var9 = false;
		}

		var8 = 1;
		break;
	}
	case 3:
	{
		if (var6)
		{
			--tp.x;
			++tp.y;
			var9 = false;
		}
		else
			++tp.x;

		var8 = 1;
		break;
	}
	case 4:
	{
		if (var6)
			++tp.z;
		else
		{
			--tp.z;
			++tp.y;
			var9 = false;
		}

		var8 = 0;
		break;
	}
	case 5:
	{
		if (var6)
		{
			++tp.z;
			++tp.y;
			var9 = false;
		}
		else
			--tp.z;

		var8 = 0;
	}
	}

	return canPower(level, tp, var6, var7, var8) ? true : var9 && canPower(level, tp.below(), var6, var7, var8);
}

bool RailTile::canPower(Level* level, const TilePos& pos, bool var5, int var6, int var7)
{
	if (RailTile::isPoweredRail(Tile::tiles[level->getTile(pos)]))
	{
		TileData var9 = level->getData(pos);
		int var10 = var9 & 7;
		if (var7 == 1 && (var10 == 0 || var10 == 4 || var10 == 5))
			return false;

		if (var7 == 0 && (var10 == 1 || var10 == 2 || var10 == 3))
			return false;

		if ((var9 & 8) != 0)
		{
			if (!level->hasNeighborSignal(pos) && !level->hasNeighborSignal(pos.above()))
				return applyPower(level, pos, var9, var5, var6 + 1);

			return true;
		}
	}

	return false;
}
