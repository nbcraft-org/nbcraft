#include "RailTile.hpp"
#include "Rail.hpp"

RailTile::RailTile(TileID id, int texture, bool isPowered)
	: Tile(id, texture, Material::decoration)
	, _m_bIsPowered(isPowered)
{
	setShape(0.0f, 0.0f, 0.0f, 1.0f, 2.0f / 16.0f, 1.0f);
	m_renderLayer = RENDER_LAYER_ALPHATEST;
}

bool RailTile::mayPlace(const TileSource& source, const TilePos& pos) const
{
	return source.isSolidBlockingTile(pos.below());
}

void RailTile::neighborChanged(TileSource& source, const TilePos& pos, TileID tile)
{
	if (source.getLevelConst().m_bIsClientSide) return;
	
	TileData data = source.getData(pos);
	TileData faceData = _getFaceData(data);

	bool badNeighbors = false; 
	if (!source.isSolidBlockingTile(pos.below()))
		badNeighbors = true;
	else if (faceData == 2 && !source.isSolidBlockingTile(pos.east()))
		badNeighbors = true;
	else if (faceData == 3 && !source.isSolidBlockingTile(pos.west()))
		badNeighbors = true;
	else if (faceData == 4 && !source.isSolidBlockingTile(pos.north()))
		badNeighbors = true;
	else if (faceData == 5 && !source.isSolidBlockingTile(pos.south()))
		badNeighbors = true;

	// if touching a non-solid tile or missing support, break the rail
	if (badNeighbors)
	{
		spawnResources(source, pos, source.getData(pos));
		source.setTile(pos, TILE_AIR);
		return;
	}

	if (_updatePower(source, pos, data))
	{
		source.updateNeighborsAt(pos.below(), m_ID);
		if (faceData == 2 || faceData == 3 || faceData == 4 || faceData == 5) {
			source.updateNeighborsAt(pos.above(), m_ID);
		}
	}
	else if (tile > 0
		&& Tile::tiles[tile]->isSignalSource()
		&& !_m_bIsPowered
		&& (Rail(source, pos)).countPotentialConnections() == 3)
	{
		_updateDir(source, pos, false);
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

AABB* RailTile::getAABB(const TileSource&, const TilePos& pos)
{
	return nullptr;
}

eRenderShape RailTile::getRenderShape() const
{
	return SHAPE_RAIL;
}

void RailTile::updateShape(const TileSource& source, const TilePos& pos)
{
	TileData data = source.getData(pos);
	if (data >= 2 && data <= 5)
		setShape(0.0f, 0.0f, 0.0f, 1.0f, 10.0f / 16.0f, 1.0f);
	else
		setShape(0.0f, 0.0f, 0.0f, 1.0f, 2.0f / 16.0f, 1.0f);
}

void RailTile::onPlace(TileSource& source, const TilePos& pos)
{
	_updateDir(source, pos, true);

	TileData data = source.getData(pos);
	if (_updatePower(source, pos, data))
	{
		TileData faceData = _getFaceData(data);
		source.updateNeighborsAt(pos.below(), m_ID);
		if (faceData == 2 || faceData == 3 || faceData == 4 || faceData == 5)
			source.updateNeighborsAt(pos.above(), m_ID);
	}
}

void RailTile::setPlacedBy(const TilePos& pos, Mob& mob)
{
	TileSource& source = mob.getTileSource();

	int rot = Mth::floor(0.5f + (mob.m_rot.yaw * 4.0f / 360.0f)) & 3;
	if (rot == 1 || rot == 3)
		source.getLevel().setData(pos, 1);

	//@NOTE: neighborChanged is called first in the original, but this was causing problems with powered rails being unpowered, certainly it's called for some reason after setPlacedBy
	_updateDir(source, pos, true);
	neighborChanged(source, pos, 0);
}

void RailTile::_updateDir(TileSource& source, const TilePos& pos, bool updateNeighbors)
{
	if (source.getLevelConst().m_bIsClientSide)
		return;

	Rail(source, pos).place(source.hasNeighborSignal(pos), updateNeighbors);
}

int RailTile::getTexture(Facing::Name face, TileData data) const
{
	if (_m_bIsPowered)
	{
		// TODO: remove isPoweredRail(this); redundant?
		if (isPoweredRail(this) && _getPowered(data) != 0)
			return m_TextureFrame + 16;
	}
	else if (data >= 6)
		return m_TextureFrame - 16; // Powered Rail but not powered?

	return m_TextureFrame; // Rail
}

bool RailTile::_applyPower(TileSource& source, const TilePos& pos, TileData data, bool isForward, int powerDistance) {
	if (powerDistance >= 8)
		return false;

	TilePos tp(pos);
	TileData faceData = _getFaceData(data);
	bool sameHeight = true;
	switch (faceData) {
	case 0:
	{
		if (isForward)
			++tp.z;
		else
			--tp.z;
		break;
	}
	case 1:
	{
		if (isForward)
			--tp.x;
		else
			++tp.x;
		break;
	}
	case 2:
	{
		if (isForward)
			--tp.x;
		else
		{
			++tp.x;
			++tp.y;
			sameHeight = false;
		}

		faceData = 1;
		break;
	}
	case 3:
	{
		if (isForward)
		{
			--tp.x;
			++tp.y;
			sameHeight = false;
		}
		else
			++tp.x;

		faceData = 1;
		break;
	}
	case 4:
	{
		if (isForward)
			++tp.z;
		else
		{
			--tp.z;
			++tp.y;
			sameHeight = false;
		}

		faceData = 0;
		break;
	}
	case 5:
	{
		if (isForward)
		{
			++tp.z;
			++tp.y;
			sameHeight = false;
		}
		else
			--tp.z;

		faceData = 0;
	}
	}

	return _canPower(source, tp, isForward, powerDistance, faceData) ? true : sameHeight && _canPower(source, tp.below(), isForward, powerDistance, faceData);
}

bool RailTile::_canPower(TileSource& source, const TilePos& pos, bool isForward, int powerDistance, int var7)
{
	if (RailTile::isPoweredRail(Tile::tiles[source.getTile(pos)]))
	{
		TileData data = source.getData(pos);
		TileData faceData = _getFaceData(data);
		if (var7 == 1 && (faceData == 0 || faceData == 4 || faceData == 5))
			return false;

		if (var7 == 0 && (faceData == 1 || faceData == 2 || faceData == 3))
			return false;

		if (_getPowered(data) != 0)
		{
			if (!source.hasNeighborSignal(pos) && !source.hasNeighborSignal(pos.above()))
				return _applyPower(source, pos, data, isForward, powerDistance + 1);

			return true;
		}
	}

	return false;
}

bool RailTile::_updatePower(TileSource& source, const TilePos& pos, TileData data)
{
	if (!isPoweredRail(this))
		return false;

	TileData faceData = _getFaceData(data);
	bool hasSignal = source.hasNeighborSignal(pos) || source.hasNeighborSignal(pos.above());
	hasSignal = hasSignal || _applyPower(source, pos, data, true, 0) || _applyPower(source, pos, data, false, 0);
	
	bool updated = false;
	if (hasSignal && _getPowered(data) == 0)
	{
		source.getLevel().setData(pos, faceData | 8);
		updated = true;
	}
	else if (!hasSignal && _getPowered(data) != 0)
	{
		source.getLevel().setData(pos, faceData);
		updated = true;
	}

	return updated;
}
