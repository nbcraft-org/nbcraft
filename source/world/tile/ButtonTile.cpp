#include "ButtonTile.hpp"
#include "world/level/Level.hpp"
#include "world/level/TileSource.hpp"
#include "world/level/TileTickingQueue.hpp"

ButtonTile::ButtonTile(TileID id, int texture) : Tile(id, texture, Material::decoration)
{
	setTicking(true);
}

AABB* ButtonTile::getAABB(const TileSource&, const TilePos& pos)
{
	return nullptr;
}

int ButtonTile::getTickDelay() const
{
	return 20;
}

bool ButtonTile::isSolidRender() const
{
	return false;
}

bool ButtonTile::isCubeShaped() const
{
	return false;
}

bool ButtonTile::mayPlace(const TileSource& source, const TilePos& pos) const
{
	if (source.isSolidBlockingTile(pos.west())) return true;
	if (source.isSolidBlockingTile(pos.east())) return true;
	if (source.isSolidBlockingTile(pos.north())) return true;
	if (source.isSolidBlockingTile(pos.south())) return true;

	return false;
}

void ButtonTile::setPlacedOnFace(TileSource& source, const TilePos& pos, Facing::Name face)
{
	TileData data = source.getData(pos);
	int var7 = data & 8;
	data &= 7;
	data = unk_h(source, pos);

	switch (face)
	{
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
	default:
		break;
	}

	/*if (face == Facing::NORTH && source.isSolidBlockingTile(pos.south()))
		data = 4;
	else if (face == Facing::SOUTH && source.isSolidBlockingTile(pos.north()))
		data = 3;
	else if (face == Facing::WEST && source.isSolidBlockingTile(pos.east()))
		data = 2;
	else if (face == Facing::EAST && source.isSolidBlockingTile(pos.west()))
		data = 1;
	else
		data = unk_h(level, pos);*/

	source.setTileAndData(pos, FullTile(this, data + var7));
}

int ButtonTile::unk_h(TileSource& source, const TilePos& pos)
{
	return source.isSolidBlockingTile(pos.west()) ? 1 : (source.isSolidBlockingTile(pos.east()) ? 2 : (source.isSolidBlockingTile(pos.north()) ? 3 : (source.isSolidBlockingTile(pos.south()) ? 4 : 1)));
}

void ButtonTile::neighborChanged(TileSource& source, const TilePos& pos, TileID tile)
{
	if (!checkCanSurvive(source, pos))
		return;

	TileData data = source.getData(pos) & 7;

	bool flag = false;
	if (data == 1 && !source.isSolidBlockingTile(pos.west())) flag = true;
	else if (data == 2 && !source.isSolidBlockingTile(pos.east())) flag = true;
	else if (data == 3 && !source.isSolidBlockingTile(pos.north())) flag = true;
	else if (data == 4 && !source.isSolidBlockingTile(pos.south())) flag = true;

	if (!flag)
		return; // all good

	spawnResources(source, pos, source.getData(pos));
	source.setTile(pos, TILE_AIR);
}

bool ButtonTile::checkCanSurvive(TileSource& source, const TilePos& pos)
{
	if (mayPlace(source, pos))
		return true;

	spawnResources(source, pos, source.getData(pos));
	source.setTile(pos, TILE_AIR);

	return false;
}

void ButtonTile::updateShape(const TileSource& source, const TilePos& pos)
{
	TileData data = source.getData(pos);
	int var6 = data & 7;
	bool var7 = (data & 8) > 0;
	float var8 = 6.0f / 16.0f;
	float var9 = 10.0f / 16.0f;
	float var10 = 3.0f / 16.0f;
	float var11 = 2.0f / 16.0f;
	if (var7)
	{
		var11 = 1.0f / 16.0f;
	}

	switch (var6)
	{
	case 1:
		setShape(0.0f, var8, 0.5f - var10, var11, var9, 0.5f + var10);
		break;
	case 2:
		setShape(1.0f - var11, var8, 0.5f - var10, 1.0f, var9, 0.5f + var10);
		break;
	case 3:
		setShape(0.5f - var10, var8, 0.0f, 0.5f + var10, var9, var11);
		break;
	case 4:
		setShape(0.5f - var10, var8, 1.0f - var11, 0.5f + var10, var9, 1.0f);
		break;
	}
}

void ButtonTile::attack(const TilePos& pos, Player& player)
{
	use(pos, player);
}

bool ButtonTile::use(const TilePos& pos, Player& player)
{
	Level& level = player.getLevel();
	TileSource& source = player.getTileSource();

	TileData data = source.getData(pos);
	int var7 = data & 7;
	int var8 = 8 - (data & 8);
	if (var8 == 0)
	{
		return true;
	}
	else
	{
		source.setTileAndData(pos, FullTile(this, var7 + var8));
		source.fireTilesDirty(pos, pos);
		level.playSound(pos + 0.5f, "random.click", 0.3f, 0.6f);
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

		TileTickingQueue* pQueue = source.getTickQueue(pos);

		pQueue->add(source, pos, m_ID, getTickDelay());
		return true;
	}
}

void ButtonTile::onRemove(TileSource& source, const TilePos& pos)
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

int ButtonTile::getSignal(const TileSource& source, const TilePos& pos, Facing::Name face) const
{
	return (source.getData(pos) & 8) > 0;
}

int ButtonTile::getDirectSignal(const TileSource& source, const TilePos& pos, Facing::Name face) const
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

bool ButtonTile::isSignalSource() const
{
	return true;
}

void ButtonTile::tick(TileSource& source, const TilePos& pos, Random* random)
{
	Level& level = source.getLevel();

	if (!level.m_bIsClientSide)
	{
		int var6 = source.getData(pos);
		if ((var6 & 8) != 0)
		{
			source.setTileAndData(pos, FullTile(this, var6 & 7));
			source.updateNeighborsAt(pos, m_ID);
			int var7 = var6 & 7;
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

			level.playSound(pos + 0.5f, "random.click", 0.3f, 0.5f);
			source.fireTilesDirty(pos, pos);
		}
	}
}

void ButtonTile::updateDefaultShape()
{
	float var1 = 3.0f / 16.0f;
	float var2 = 2.0f / 16.0f;
	//float var3 = 2.0f / 16.0f;
	setShape(0.5f - var1, 0.5f - var2, 0.5f - var2, 0.5f + var1, 0.5f + var2, 0.5f + var2);
}
