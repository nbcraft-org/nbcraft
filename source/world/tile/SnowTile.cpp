#include "SnowTile.hpp"
#include "world/level/TileSource.hpp"

SnowTile::SnowTile(TileID id, int texture) : Tile(id, texture, Material::snow)
{
	setTicking(true);
}

int SnowTile::getResource(TileData data, Random* random) const
{
	return Item::snowBall->m_itemID;
}

int SnowTile::getResourceCount(Random* random) const
{
	return 4;
}

void SnowTile::tick(TileSource& source, const TilePos& pos, Random* random)
{
	if (source.getBrightness(LightLayer::Block, pos) > 11)
	{
		spawnResources(source, pos, source.getData(pos));
		source.setTile(pos, TILE_AIR);
	}
}
