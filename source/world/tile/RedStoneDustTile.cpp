#include "RedStoneDustTile.hpp"
#include "world/level/TileSource.hpp"
#include "world/level/Level.hpp"

RedStoneDustTile::RedStoneDustTile(TileID id, int texture) : Tile(id, texture, Material::decoration)
{
	m_bShouldSignal = true;
	m_TextureFrame = texture;
	m_renderLayer = RENDER_LAYER_ALPHATEST;
	setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f / 16.0f, 1.0f);
}

int RedStoneDustTile::getTexture(Facing::Name face, TileData data) const
{
	return m_TextureFrame;
}

AABB* RedStoneDustTile::getAABB(TileSource& source, const TilePos& pos)
{
	return nullptr;
}

bool RedStoneDustTile::isSolidRender() const
{
	return false;
}

bool RedStoneDustTile::isCubeShaped() const
{
	return false;
}

eRenderShape RedStoneDustTile::getRenderShape() const
{
	return SHAPE_DUST;
}

bool RedStoneDustTile::mayPlace(TileSource& source, const TilePos& pos) const
{
	return source.isSolidBlockingTile(pos.below());
}

void RedStoneDustTile::updatePowerStrength(TileSource& source, const TilePos& pos)
{
	updatePowerStrength(source, pos, pos);
	std::vector<TilePos> var5;
	var5.insert(var5.begin(), m_toUpdate.begin(), m_toUpdate.end());
	m_toUpdate.clear();

	for (int var6 = 0; var6 < (int)var5.size(); var6++)
	{
		TilePos var7 = var5[var6];
		source.updateNeighborsAt(var7, m_ID);
	}
}

void RedStoneDustTile::updatePowerStrength(TileSource& source, const TilePos& pos1, const TilePos& pos2)
{
	TileData var8 = source.getData(pos1);
	int var9 = 0;
	m_bShouldSignal = false;
	bool var10 = source.hasNeighborSignal(pos1);
	m_bShouldSignal = true;
	int var11;
	int var12;
	int var13;
	if (var10)
	{
		var9 = 15;
	}
	else 
	{
		for (var11 = 0; var11 < 4; var11++)
		{
			var12 = pos1.x;
			var13 = pos1.z;
			if (var11 == 0)
			{
				var12 = pos1.x - 1;
			}

			if (var11 == 1)
			{
				var12++;
			}

			if (var11 == 2)
			{
				var13 = pos1.z - 1;
			}

			if (var11 == 3)
			{
				var13++;
			}

			if (var12 != pos2.x || pos1.y != pos2.y || var13 != pos2.z)
			{
				var9 = checkTarget(source, TilePos(var12, pos1.y, var13), var9);
			}

			if (source.isSolidBlockingTile(TilePos(var12, pos1.y, var13)) && !source.isSolidBlockingTile(pos1.above()))
			{
				if (var12 != pos2.x || pos1.y + 1 != pos2.y || var13 != pos2.z)
				{
					var9 = checkTarget(source, TilePos(var12, pos1.y + 1, var13), var9);
				}
			}
			else if (!source.isSolidBlockingTile(TilePos(var12, pos1.y, var13)) && (var12 != pos2.x || pos1.y - 1 != pos2.y || var13 != pos2.z))
			{
				var9 = checkTarget(source, TilePos(var12, pos1.y - 1, var13), var9);
			}
		}

		if (var9 > 0)
		{
			var9--;
		}
		else
		{
			var9 = 0;
		}
	}

	if (var8 != var9)
	{
		source.m_bNoNeighborUpdate = true;
		source.setTileAndData(pos1, FullTile(this, var9));
		source.setTilesDirty(pos1, pos1);
		source.m_bNoNeighborUpdate = false;

		for (var11 = 0; var11 < 4; var11++)
		{
			var12 = pos1.x;
			var13 = pos1.z;
			int var14 = pos1.y - 1;
			if (var11 == 0)
			{
				var12 = pos1.x - 1;
			}

			if (var11 == 1)
			{
				var12++;
			}

			if (var11 == 2)
			{
				var13 = pos1.z - 1;
			}

			if (var11 == 3)
			{
				var13++;
			}

			if (source.isSolidBlockingTile(TilePos(var12, pos1.y, var13)))
			{
				var14 += 2;
			}

			//bool var15 = false;
			int var16 = checkTarget(source, TilePos(var12, pos1.y, var13), -1);
			var9 = source.getData(pos1);
			if (var9 > 0)
			{
				var9--;
			}

			if (var16 >= 0 && var16 != var9)
			{
				updatePowerStrength(source, TilePos(var12, pos1.y, var13), pos1);
			}

			var16 = checkTarget(source, TilePos(var12, var14, var13), -1);
			var9 = source.getData(pos1);
			if (var9 > 0)
			{
				var9--;
			}

			if (var16 >= 0 && var16 != var9)
			{
				updatePowerStrength(source, TilePos(var12, var14, var13), pos1);
			}
		}

		if (var8 == 0 || var9 == 0)
		{
			m_toUpdate.insert(pos1);
			m_toUpdate.insert(pos1.west());
			m_toUpdate.insert(pos1.east());
			m_toUpdate.insert(pos1.below());
			m_toUpdate.insert(pos1.above());
			m_toUpdate.insert(pos1.north());
			m_toUpdate.insert(pos1.south());
		}
	}
}

void RedStoneDustTile::checkCornerChangeAt(TileSource& source, const TilePos& pos)
{
	if (source.getTile(pos) == m_ID)
	{
		source.updateNeighborsAt(pos, m_ID);
		source.updateNeighborsAt(pos.west(), m_ID);
		source.updateNeighborsAt(pos.east(), m_ID);
		source.updateNeighborsAt(pos.north(), m_ID);
		source.updateNeighborsAt(pos.south(), m_ID);
		source.updateNeighborsAt(pos.below(), m_ID);
		source.updateNeighborsAt(pos.above(), m_ID);
	}
}

void RedStoneDustTile::onPlace(TileSource& source, const TilePos& pos)
{
	Level& level = source.getLevel();

	Tile::onPlace(source, pos);
	if (!level.m_bIsClientSide)
	{
		updatePowerStrength(source, pos);
		source.updateNeighborsAt(pos.above(), m_ID);
		source.updateNeighborsAt(pos.below(), m_ID);
		checkCornerChangeAt(source, pos.west());
		checkCornerChangeAt(source, pos.east());
		checkCornerChangeAt(source, pos.north());
		checkCornerChangeAt(source, pos.south());
		if (source.isSolidBlockingTile(pos.west()))
		{
			checkCornerChangeAt(source, pos.west().above());
		}
		else
		{
			checkCornerChangeAt(source, pos.west().below());
		}

		if (source.isSolidBlockingTile(pos.east()))
		{
			checkCornerChangeAt(source, pos.east().above());
		}
		else
		{
			checkCornerChangeAt(source, pos.east().below());
		}

		if (source.isSolidBlockingTile(pos.north()))
		{
			checkCornerChangeAt(source, pos.above().north());
		}
		else
		{
			checkCornerChangeAt(source, pos.below().north());
		}

		if (source.isSolidBlockingTile(pos.south()))
		{
			checkCornerChangeAt(source, pos.above().south());
		}
		else
		{
			checkCornerChangeAt(source, pos.below().south());
		}
	}
}

void RedStoneDustTile::onRemove(TileSource& source, const TilePos& pos)
{
	Level& level = source.getLevel();

	Tile::onRemove(source, pos);
	if (!level.m_bIsClientSide)
	{
		source.updateNeighborsAt(pos.above(), m_ID);
		source.updateNeighborsAt(pos.below(), m_ID);
		updatePowerStrength(source, pos);
		checkCornerChangeAt(source, pos.west());
		checkCornerChangeAt(source, pos.east());
		checkCornerChangeAt(source, pos.north());
		checkCornerChangeAt(source, pos.south());
		if (source.isSolidBlockingTile(pos.west()))
		{
			checkCornerChangeAt(source, pos.west().above());
		}
		else
		{
			checkCornerChangeAt(source, pos.west().below());
		}

		if (source.isSolidBlockingTile(pos.east()))
		{
			checkCornerChangeAt(source, pos.east().above());
		}
		else
		{
			checkCornerChangeAt(source, pos.east().below());
		}

		if (source.isSolidBlockingTile(pos.north()))
		{
			checkCornerChangeAt(source, pos.above().north());
		}
		else
		{
			checkCornerChangeAt(source, pos.below().north());
		}

		if (source.isSolidBlockingTile(pos.south()))
		{
			checkCornerChangeAt(source, pos.above().south());
		}
		else
		{
			checkCornerChangeAt(source, pos.below().south());
		}
	}
}

int RedStoneDustTile::checkTarget(TileSource& source, const TilePos& pos, int something)
{
	if (source.getTile(pos) != m_ID)
	{
		return something;
	}
	else
	{
		int var6 = source.getData(pos);
		return var6 > something ? var6 : something;
	}
}

void RedStoneDustTile::neighborChanged(TileSource& source, const TilePos& pos, TileID tile)
{
	Level& level = source.getLevel();

	if (!level.m_bIsClientSide)
	{
		int var6 = source.getData(pos);
		bool var7 = mayPlace(source, pos);
		if (!var7)
		{
			spawnResources(source, pos, var6);
			source.setTile(pos, 0);
		}
		else
		{
			updatePowerStrength(source, pos);
		}

		Tile::neighborChanged(source, pos, tile);
	}
}

int RedStoneDustTile::getResource(TileData data, Random* random) const
{
	return Item::redStone->m_itemID;
}

int RedStoneDustTile::getDirectSignal(TileSource& source, const TilePos& pos, Facing::Name face) const
{
	return !m_bShouldSignal ? 0 : getSignal(source, pos, face);
}

int RedStoneDustTile::getSignal(TileSource& source, const TilePos& pos, Facing::Name face) const
{
	if (!m_bShouldSignal)
	{
		return 0;
	}
	else if (source.getData(pos) == 0)
	{
		return 0;
	}
	else if (face == Facing::UP)
	{
		return true;
	}
	else
	{
		bool var6 = shouldConnectTo(source, pos.west()) || (!source.isSolidBlockingTile(pos.west()) && shouldConnectTo(source, pos.west().below()));
		bool var7 = shouldConnectTo(source, pos.east()) || (!source.isSolidBlockingTile(pos.east()) && shouldConnectTo(source, pos.east().below()));
		bool var8 = shouldConnectTo(source, pos.north()) || (!source.isSolidBlockingTile(pos.north()) && shouldConnectTo(source, pos.below().north()));
		bool var9 = shouldConnectTo(source, pos.south()) || (!source.isSolidBlockingTile(pos.south()) && shouldConnectTo(source, pos.below().south()));
		if (!source.isSolidBlockingTile(pos.above()))
		{
			if (source.isSolidBlockingTile(pos.west()) && shouldConnectTo(source, pos.west().above()))
			{
				var6 = true;
			}

			if (source.isSolidBlockingTile(pos.east()) && shouldConnectTo(source, pos.east().above()))
			{
				var7 = true;
			}

			if (source.isSolidBlockingTile(pos.north()) && shouldConnectTo(source, pos.above().north()))
			{
				var8 = true;
			}

			if (source.isSolidBlockingTile(pos.south()) && shouldConnectTo(source, pos.above().south()))
			{
				var9 = true;
			}
		}

		return !var8 && !var7 && !var6 && !var9 && face >= Facing::NORTH && face <= Facing::EAST ? true : (face == Facing::NORTH && var8 && !var6 && !var7 ? true : (face == Facing::SOUTH && var9 && !var6 && !var7 ? true : (face == Facing::WEST && var6 && !var8 && !var9 ? true : face == Facing::EAST && var7 && !var8 && !var9)));
	}
}

bool RedStoneDustTile::isSignalSource() const
{
	return m_bShouldSignal;
}

void RedStoneDustTile::animateTick(TileSource& source, const TilePos& pos, Random* random)
{
	int var6 = source.getData(pos);
	if (var6 > 0)
	{
		float var7 = float(pos.x) + 0.5f + (random->nextFloat() - 0.5f) * 0.2f;
		float var9 = float(pos.y) + 1.0f / 16.0f;
		float var11 = float(pos.z) + 0.5f + (random->nextFloat() - 0.5f) * 0.2f;
		/*float var13 = float(var6) / 15.0f;
		float var14 = var13 * 0.6f + 0.4f;
		if (var6 == 0) // either the java decompiler or notch shat itself
		{
			var14 = 0.0f;
		}

		float var15 = var13 * var13 * 0.7f - 0.5f;
		float var16 = var13 * var13 * 0.6f - 0.7f;
		if (var15 < 0.0f)
		{
			var15 = 0.0f;
		}

		if (var16 < 0.0f)
		{
			var16 = 0.0f;
		}
		*/

		Level& level = source.getLevel();
		level.addParticle("reddust", Vec3(var7, var9, var11), Vec3::ZERO); // var14, var15, var16
	}
}

bool RedStoneDustTile::shouldConnectTo(TileSource& source, const TilePos& pos)
{
	TileID var4 = source.getTile(pos);
	return var4 == Tile::redStoneDust->m_ID ? true : (var4 == 0 ? false : Tile::tiles[var4]->isSignalSource());
}
