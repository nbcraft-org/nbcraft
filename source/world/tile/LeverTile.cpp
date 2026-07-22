#include "LeverTile.hpp"
#include "world/level/Level.hpp"
#include "world/level/TileSource.hpp"

LeverTile::LeverTile(TileID id, int texture) : Tile(id, texture, Material::decoration)
{
	m_renderLayer = RENDER_LAYER_ALPHATEST;
}

AABB* LeverTile::getAABB(const TileSource& source, const TilePos& pos)
{
	return nullptr;
}

bool LeverTile::isSolidRender() const
{
	return false;
}

bool LeverTile::isCubeShaped() const
{
	return false;
}

eRenderShape LeverTile::getRenderShape() const
{
	return SHAPE_LEVER;
}

bool LeverTile::mayPlace(const TileSource& source, const TilePos& pos) const
{
	if (source.isSolidBlockingTile(pos.west())) return true;
	if (source.isSolidBlockingTile(pos.east())) return true;
	if (source.isSolidBlockingTile(pos.north())) return true;
	if (source.isSolidBlockingTile(pos.south())) return true;
	if (source.isSolidBlockingTile(pos.below())) return true;

	return false;
}

void LeverTile::setPlacedOnFace(TileSource& source, const TilePos& pos, Facing::Name face)
{
	Level& level = source.getLevel();

	TileData data = source.getData(pos);
	int var7 = data & 8;
	data &= 7;

	switch (face)
	{
	case Facing::UP:
		if (source.isSolidBlockingTile(pos.below()))
			data = 5 + level.m_random.nextInt(2);
		break;
	case Facing::NORTH:
		if (source.isSolidBlockingTile(pos.south()))
			data = 4;
		break;
	case Facing::SOUTH:
		if (source.isSolidBlockingTile(pos.north()))
			data = 3;
		break;
	case Facing::WEST:
		if (source.isSolidBlockingTile(pos.east()))
			data = 2;
		break;
	case Facing::EAST:
		if (source.isSolidBlockingTile(pos.west()))
			data = 1;
		break;
	default: break;
	}

	source.setTileAndData(pos, FullTile(this, data + var7));
}

void LeverTile::neighborChanged(TileSource& source, const TilePos& pos, TileID tile)
{
	if (!_checkCanSurvive(source, pos))
		return;

	TileData data = source.getData(pos) & 7;

	bool flag = false;
	if (data == 1 && !source.isSolidBlockingTile(pos.west())) flag = true;
	else if (data == 2 && !source.isSolidBlockingTile(pos.east())) flag = true;
	else if (data == 3 && !source.isSolidBlockingTile(pos.north())) flag = true;
	else if (data == 4 && !source.isSolidBlockingTile(pos.south())) flag = true;
	else if (data == 5 && !source.isSolidBlockingTile(pos.below())) flag = true;

	if (!flag)
		return; // all good

	spawnResources(source, pos, source.getData(pos));
	source.setTile(pos, TILE_AIR);
}

bool LeverTile::_checkCanSurvive(TileSource& source, const TilePos& pos)
{
	if (mayPlace(source, pos))
		return true;

	spawnResources(source, pos, source.getData(pos));
	source.setTile(pos, TILE_AIR);

	return false;
}

void LeverTile::updateShape(const TileSource& source, const TilePos& pos)
{
	TileData data = source.getData(pos) & 7;
	float var6 = 3.0f / 16.0f;

	switch (data)
	{
	case 1:
		setShape(0.0f, 0.2f, 0.5f - var6, var6 * 2.0f, 0.8f, 0.5f + var6);
		break;
	case 2:
		setShape(1.0f - var6 * 2.0f, 0.2f, 0.5f - var6, 1.0f, 0.8f, 0.5f + var6);
		break;
	case 3:
		setShape(0.5f - var6, 0.2f, 0.0f, 0.5f + var6, 0.8f, var6 * 2.0f);
		break;
	case 4:
		setShape(0.5f - var6, 0.2f, 1.0f - var6 * 2.0f, 0.5f + var6, 0.8f, 1.0f);
		break;
	default:
		var6 = 0.25f;
		setShape(0.5f - var6, 0.0f, 0.5f - var6, 0.5f + var6, 0.6f, 0.5f + var6);
		break;
	}
}

void LeverTile::attack(const TilePos& pos, Player& player)
{
	use(pos, player);
}

bool LeverTile::use(const TilePos& pos, Player& player)
{
	Level& level = player.getLevel();
	TileSource& source = player.getTileSource();

	if (level.m_bIsClientSide)
		return true;
	
	TileData data = source.getData(pos);
	int var7 = data & 7;
	int var8 = 8 - (data & 8);
	source.setTileAndData(pos, FullTile(this, var7 + var8));
	source.fireTilesDirty(pos, pos);
	level.playSound(pos + 0.5f, "random.click", 0.3f, var8 > 0 ? 0.6f : 0.5f);
	source.updateNeighborsAt(pos, m_ID);
	switch (var7)
	{
	case 1:
		source.updateNeighborsAt(pos.west(), m_ID);
		break;
	case 2:
		source.updateNeighborsAt(pos.east(), m_ID);
		break;
	case 3:
		source.updateNeighborsAt(pos.north(), m_ID);
		break;
	case 4:
		source.updateNeighborsAt(pos.south(), m_ID);
		break;
	default:
		source.updateNeighborsAt(pos.below(), m_ID);
		break;
	}

	return true;
}

void LeverTile::onRemove(TileSource& source, const TilePos& pos)
{
	TileData data = source.getData(pos);
	if ((data & 8) > 0)
	{
		source.updateNeighborsAt(pos, m_ID);
		int var6 = data & 7;
		switch (var6)
		{
		case 1:
			source.updateNeighborsAt(pos.west(), m_ID);
			break;
		case 2:
			source.updateNeighborsAt(pos.east(), m_ID);
			break;
		case 3:
			source.updateNeighborsAt(pos.north(), m_ID);
			break;
		case 4:
			source.updateNeighborsAt(pos.south(), m_ID);
			break;
		default:
			source.updateNeighborsAt(pos.below(), m_ID);
			break;
		}
	}

	Tile::onRemove(source, pos);
}

int LeverTile::getSignal(const TileSource& source, const TilePos& pos, Facing::Name face) const
{
	return (source.getData(pos) & 8) > 0;
}

int LeverTile::getDirectSignal(const TileSource& source, const TilePos& pos, Facing::Name face) const
{
	TileData data = source.getData(pos);
	if ((data & 8) == 0) 
	{
		return false;
	}
	else 
	{
		int var7 = data & 7;
		if (var7 == 5 && face == Facing::UP) 
		{
			return true;
		}
		else if (var7 == 4 && face == Facing::NORTH) 
		{
			return true;
		}
		else if (var7 == 3 && face == Facing::SOUTH) 
		{
			return true;
		}
		else 
		{
			return var7 == 2 && face == Facing::WEST ? true : var7 == 1 && face == Facing::EAST;
		}
	}
}

bool LeverTile::isSignalSource() const
{
	return true;
}
