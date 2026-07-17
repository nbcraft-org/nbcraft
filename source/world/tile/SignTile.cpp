#include "SignTile.hpp"
#include "world/level/Level.hpp"
#include "entity/SignTileEntity.hpp"

SignTile::SignTile(TileID id, bool isWall) 
	: EntityTile(id, TEXTURE_PLANKS, Material::wood)
	, m_bIsWall(isWall)
{
	setShape(0.25f, 0.0f, 0.25f, 0.75f, 1.0f, 0.75f);
}

void SignTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	bool canDrop = false;
	if (!m_bIsWall)
	{
		if (!level->getMaterial(pos.below())->isSolid())
			canDrop = true;
	}
	else
	{
		TileData tileData = level->getData(pos);
		canDrop = true;
		if (tileData == 2 && level->getMaterial(pos.south())->isSolid())
			canDrop = false;

		if (tileData == 3 && level->getMaterial(pos.north())->isSolid())
			canDrop = false;

		if (tileData == 4 && level->getMaterial(pos.east())->isSolid())
			canDrop = false;

		if (tileData == 5 && level->getMaterial(pos.west())->isSolid())
			canDrop = false;
	}

	if (canDrop)
	{
		spawnResources(level, pos, level->getData(pos));
		level->setTile(pos, TILE_AIR);
	}

	Tile::neighborChanged(level, pos, tile);
}

bool SignTile::isSolidRender() const
{
	return false;
}

bool SignTile::isCubeShaped() const
{
	return false;
}

AABB* SignTile::getAABB(const Level* level, const TilePos& pos)
{
	return nullptr;
}

eRenderShape SignTile::getRenderShape() const
{
	return SHAPE_NONE;
}

bool SignTile::hasTileEntity() const
{
	return true;
}

TileEntity* SignTile::newTileEntity()
{
	return new SignTileEntity;
}

void SignTile::updateShape(const LevelSource* level, const TilePos& pos)
{
	if (!m_bIsWall) return;

	TileData tileData = level->getData(pos);
	constexpr float var6 = 9.0f / 32.0f;
	constexpr float var7 = 25.0f / 32.0f;
	constexpr float var8 = 0.0f;
	constexpr float var9 = 1.0f;
	constexpr float var10 = 2.0f / 16.0f;
	setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	if (tileData == 2) {
		setShape(var8, var6, 1.0f - var10, var9, var7, 1.0f);
	}

	if (tileData == 3) {
		setShape(var8, var6, 0.0f, var9, var7, var10);
	}

	if (tileData == 4) {
		setShape(1.0f - var10, var6, var8, 1.0f, var7, var9);
	}

	if (tileData == 5) {
		setShape(0.0f, var6, var8, var10, var7, var9);
	}

}

int SignTile::getResource(TileData, Random*) const
{
	return Item::sign->m_itemID;
}
