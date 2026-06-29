#include "SnowTile.hpp"
#include "world/level/Level.hpp"

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

void SnowTile::tick(Level* level, const TilePos& pos, Random* random)
{
	if (level->getBrightness(LightLayer::Block, pos) > 11)
	{
		spawnResources(level, pos, level->getData(pos));
		level->setTile(pos, TILE_AIR);
	}
}
