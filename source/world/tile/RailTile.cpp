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
	TileData faceData = getFaceData(data);

	bool badNeighbors = false; 
	if (!source.isSolidBlockingTile(pos.below()))
		badNeighbors = true;
	else if (faceData == RailTile::WEST_EAST_ABOVE && !source.isSolidBlockingTile(pos.east()))
		badNeighbors = true;
	else if (faceData == RailTile::EAST_WEST_ABOVE && !source.isSolidBlockingTile(pos.west()))
		badNeighbors = true;
	else if (faceData == RailTile::SOUTH_NORTH_ABOVE && !source.isSolidBlockingTile(pos.north()))
		badNeighbors = true;
	else if (faceData == RailTile::NORTH_SOUTH_ABOVE && !source.isSolidBlockingTile(pos.south()))
		badNeighbors = true;

	// if touching a non-solid tile or missing support, break the rail
	if (badNeighbors)
	{
		spawnResources(source, pos, source.getData(pos));
		source.setTile(pos, TILE_AIR);
		return;
	}

	if (!_updatePower(source, pos, data)
		&& tile > 0
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
	_updatePower(source, pos, source.getData(pos));
}

void RailTile::setPlacedBy(const TilePos& pos, Mob& mob)
{
	TileSource& source = mob.getTileSource();

	int rot = Mth::floor(0.5f + (mob.m_rot.yaw * 4.0f / 360.0f)) & 3;
	if (rot == 1 || rot == 3)
		source.getLevel().setData(pos, RailTile::WEST_EAST);

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
	static constexpr int C_TEXTURE_OFFSET = 16;
	if (_m_bIsPowered)
	{
		if (getPowered(data))
			return m_TextureFrame + C_TEXTURE_OFFSET;
	}
	else if (data >= EAST_SOUTH) // curved rail
		return m_TextureFrame - C_TEXTURE_OFFSET;

	return m_TextureFrame;
}

bool RailTile::_applyPower(TileSource& source, const TilePos& pos, TileData data, bool inverted, int powerDistance) {
	if (powerDistance >= 8)
		return false;

	TilePos tp(pos);
	TileData faceData = getFaceData(data);
	bool sameHeight = true;
	switch (faceData) {
	case 0:
	{
		if (inverted)
			++tp.z;
		else
			--tp.z;
		break;
	}
	case 1:
	{
		if (inverted)
			--tp.x;
		else
			++tp.x;
		break;
	}
	case 2:
	{
		if (inverted)
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
		if (inverted)
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
		if (inverted)
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
		if (inverted)
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

	return _canPower(source, tp, inverted, powerDistance, faceData) ? true : sameHeight && _canPower(source, tp.below(), inverted, powerDistance, faceData);
}

bool RailTile::_canPower(TileSource& source, const TilePos& pos, bool inverted, int powerDistance, int oFaceData)
{
	if (RailTile::isPoweredRail(Tile::tiles[source.getTile(pos)]))
	{
		TileData data = source.getData(pos);
		TileData faceData = getFaceData(data);
		if (oFaceData == WEST_EAST && (faceData == NORTH_SOUTH || faceData == SOUTH_NORTH_ABOVE || faceData == NORTH_SOUTH_ABOVE))
			return false;

		if (oFaceData == NORTH_SOUTH && (faceData == WEST_EAST || faceData == WEST_EAST_ABOVE || faceData == EAST_WEST_ABOVE))
			return false;

		if (getPowered(data))
		{
			if (!source.hasNeighborSignal(pos) && !source.hasNeighborSignal(pos.above()))
				return _applyPower(source, pos, data, inverted, powerDistance + 1);

			return true;
		}
	}

	return false;
}

bool RailTile::_updatePower(TileSource& source, const TilePos& pos, TileData data)
{
	if (!isPoweredRail(this))
		return false;

	TileData faceData = getFaceData(data);
	bool hasSignal = source.hasNeighborSignal(pos) || source.hasNeighborSignal(pos.above());
	hasSignal = hasSignal || _applyPower(source, pos, data, true, 0) || _applyPower(source, pos, data, false, 0);
	
	if (hasSignal && !getPowered(data))
		source.getLevel().setData(pos, faceData | 8);
	else if (!hasSignal && getPowered(data))
		source.getLevel().setData(pos, faceData);
	else
		return false;

	source.updateNeighborsAt(pos.below(), m_ID);
	if (faceData == RailTile::WEST_EAST_ABOVE 
		|| faceData == RailTile::EAST_WEST_ABOVE 
		|| faceData == RailTile::SOUTH_NORTH_ABOVE 
		|| faceData == RailTile::NORTH_SOUTH_ABOVE)
		source.updateNeighborsAt(pos.above(), m_ID);

	return true;
}
