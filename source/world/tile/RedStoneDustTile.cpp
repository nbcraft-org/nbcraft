#include "RedStoneDustTile.hpp"
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

AABB* RedStoneDustTile::getAABB(const Level* level, const TilePos& pos)
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

bool RedStoneDustTile::mayPlace(const Level* level, const TilePos& pos) const
{
	return level->isSolidTile(pos.below());
}

void RedStoneDustTile::updatePowerStrength(Level* level, const TilePos& pos)
{
	updatePowerStrength(level, pos, pos);
	std::vector<TilePos> var5;
	var5.insert(var5.begin(), m_toUpdate.begin(), m_toUpdate.end());
	m_toUpdate.clear();

	for (int var6 = 0; var6 < (int)var5.size(); var6++)
	{
		TilePos var7 = var5[var6];
		level->updateNeighborsAt(var7, m_ID);
	}
}

void RedStoneDustTile::updatePowerStrength(Level* level, const TilePos& pos1, const TilePos& pos2)
{
	int var8 = level->getData(pos1);
	int var9 = 0;
	m_bShouldSignal = false;
	bool var10 = level->hasNeighborSignal(pos1);
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
				var9 = checkTarget(level, TilePos(var12, pos1.y, var13), var9);
			}

			if (level->isSolidTile(TilePos(var12, pos1.y, var13)) && !level->isSolidTile(pos1.above()))
			{
				if (var12 != pos2.x || pos1.y + 1 != pos2.y || var13 != pos2.z)
				{
					var9 = checkTarget(level, TilePos(var12, pos1.y + 1, var13), var9);
				}
			}
			else if (!level->isSolidTile(TilePos(var12, pos1.y, var13)) && (var12 != pos2.x || pos1.y - 1 != pos2.y || var13 != pos2.z))
			{
				var9 = checkTarget(level, TilePos(var12, pos1.y - 1, var13), var9);
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
		level->field_30 = true;
		level->setData(pos1, var9);
		level->setTilesDirty(pos1, pos1);
		level->field_30 = false;

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

			if (level->isSolidTile(TilePos(var12, pos1.y, var13)))
			{
				var14 += 2;
			}

			//bool var15 = false;
			int var16 = checkTarget(level, TilePos(var12, pos1.y, var13), -1);
			var9 = level->getData(pos1);
			if (var9 > 0)
			{
				var9--;
			}

			if (var16 >= 0 && var16 != var9)
			{
				updatePowerStrength(level, TilePos(var12, pos1.y, var13), pos1);
			}

			var16 = checkTarget(level, TilePos(var12, var14, var13), -1);
			var9 = level->getData(pos1);
			if (var9 > 0)
			{
				var9--;
			}

			if (var16 >= 0 && var16 != var9)
			{
				updatePowerStrength(level, TilePos(var12, var14, var13), pos1);
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

void RedStoneDustTile::checkCornerChangeAt(Level* level, const TilePos& pos)
{
	if (level->getTile(pos) == m_ID)
	{
		level->updateNeighborsAt(pos, m_ID);
		level->updateNeighborsAt(pos.west(), m_ID);
		level->updateNeighborsAt(pos.east(), m_ID);
		level->updateNeighborsAt(pos.north(), m_ID);
		level->updateNeighborsAt(pos.south(), m_ID);
		level->updateNeighborsAt(pos.below(), m_ID);
		level->updateNeighborsAt(pos.above(), m_ID);
	}
}

void RedStoneDustTile::onPlace(Level* level, const TilePos& pos)
{
	Tile::onPlace(level, pos);
	if (!level->m_bIsClientSide)
	{
		updatePowerStrength(level, pos);
		level->updateNeighborsAt(pos.above(), m_ID);
		level->updateNeighborsAt(pos.below(), m_ID);
		checkCornerChangeAt(level, pos.west());
		checkCornerChangeAt(level, pos.east());
		checkCornerChangeAt(level, pos.north());
		checkCornerChangeAt(level, pos.south());
		if (level->isSolidTile(pos.west()))
		{
			checkCornerChangeAt(level, pos.west().above());
		}
		else
		{
			checkCornerChangeAt(level, pos.west().below());
		}

		if (level->isSolidTile(pos.east()))
		{
			checkCornerChangeAt(level, pos.east().above());
		}
		else
		{
			checkCornerChangeAt(level, pos.east().below());
		}

		if (level->isSolidTile(pos.north()))
		{
			checkCornerChangeAt(level, pos.above().north());
		}
		else
		{
			checkCornerChangeAt(level, pos.below().north());
		}

		if (level->isSolidTile(pos.south()))
		{
			checkCornerChangeAt(level, pos.above().south());
		}
		else
		{
			checkCornerChangeAt(level, pos.below().south());
		}
	}
}

void RedStoneDustTile::onRemove(Level* level, const TilePos& pos)
{
	Tile::onRemove(level, pos);
	if (!level->m_bIsClientSide)
	{
		level->updateNeighborsAt(pos.above(), m_ID);
		level->updateNeighborsAt(pos.below(), m_ID);
		updatePowerStrength(level, pos);
		checkCornerChangeAt(level, pos.west());
		checkCornerChangeAt(level, pos.east());
		checkCornerChangeAt(level, pos.north());
		checkCornerChangeAt(level, pos.south());
		if (level->isSolidTile(pos.west()))
		{
			checkCornerChangeAt(level, pos.west().above());
		}
		else
		{
			checkCornerChangeAt(level, pos.west().below());
		}

		if (level->isSolidTile(pos.east()))
		{
			checkCornerChangeAt(level, pos.east().above());
		}
		else
		{
			checkCornerChangeAt(level, pos.east().below());
		}

		if (level->isSolidTile(pos.north()))
		{
			checkCornerChangeAt(level, pos.above().north());
		}
		else
		{
			checkCornerChangeAt(level, pos.below().north());
		}

		if (level->isSolidTile(pos.south()))
		{
			checkCornerChangeAt(level, pos.above().south());
		}
		else
		{
			checkCornerChangeAt(level, pos.below().south());
		}
	}
}

int RedStoneDustTile::checkTarget(Level* level, const TilePos& pos, int something)
{
	if (level->getTile(pos) != m_ID)
	{
		return something;
	}
	else
	{
		int var6 = level->getData(pos);
		return var6 > something ? var6 : something;
	}
}

void RedStoneDustTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	if (!level->m_bIsClientSide)
	{
		int var6 = level->getData(pos);
		bool var7 = mayPlace(level, pos);
		if (!var7)
		{
			spawnResources(level, pos, var6);
			level->setTile(pos, 0);
		}
		else
		{
			updatePowerStrength(level, pos);
		}

		Tile::neighborChanged(level, pos, tile);
	}
}

int RedStoneDustTile::getResource(TileData data, Random* random) const
{
	return Item::redStone->m_itemID;
}

int RedStoneDustTile::getDirectSignal(const Level* level, const TilePos& pos, Facing::Name face) const
{
	return !m_bShouldSignal ? 0 : getSignal(level, pos, face);
}

int RedStoneDustTile::getSignal(const LevelSource* level, const TilePos& pos, Facing::Name face) const
{
	if (!m_bShouldSignal)
	{
		return 0;
	}
	else if (level->getData(pos) == 0)
	{
		return 0;
	}
	else if (face == Facing::UP)
	{
		return true;
	}
	else
	{
		bool var6 = shouldConnectTo(level, pos.west()) || (!level->isSolidTile(pos.west()) && shouldConnectTo(level, pos.west().below()));
		bool var7 = shouldConnectTo(level, pos.east()) || (!level->isSolidTile(pos.east()) && shouldConnectTo(level, pos.east().below()));
		bool var8 = shouldConnectTo(level, pos.north()) || (!level->isSolidTile(pos.north()) && shouldConnectTo(level, pos.below().north()));
		bool var9 = shouldConnectTo(level, pos.south()) || (!level->isSolidTile(pos.south()) && shouldConnectTo(level, pos.below().south()));
		if (!level->isSolidTile(pos.above()))
		{
			if (level->isSolidTile(pos.west()) && shouldConnectTo(level, pos.west().above()))
			{
				var6 = true;
			}

			if (level->isSolidTile(pos.east()) && shouldConnectTo(level, pos.east().above()))
			{
				var7 = true;
			}

			if (level->isSolidTile(pos.north()) && shouldConnectTo(level, pos.above().north()))
			{
				var8 = true;
			}

			if (level->isSolidTile(pos.south()) && shouldConnectTo(level, pos.above().south()))
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

void RedStoneDustTile::animateTick(Level* level, const TilePos& pos, Random* random)
{
	int var6 = level->getData(pos);
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

		level->addParticle("reddust", Vec3(var7, var9, var11), Vec3::ZERO); // var14, var15, var16
	}
}

bool RedStoneDustTile::shouldConnectTo(const LevelSource* level, const TilePos& pos)
{
	int var4 = level->getTile(pos);
	return var4 == Tile::redStoneDust->m_ID ? true : (var4 == 0 ? false : Tile::tiles[var4]->isSignalSource());
}
