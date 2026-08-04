#include "EntityTile.hpp"
#include "world/level/TileSource.hpp"

EntityTile::EntityTile(TileID id, int textureID, Material *material)  : Tile(id, textureID, material)
{
}

bool EntityTile::hasTileEntity() const
{
    return true;
}

void EntityTile::onPlace(TileSource& source, const TilePos &pos)
{
    Tile::onPlace(source, pos);
    source.setTileEntity(pos, newTileEntity());
}

void EntityTile::onRemove(TileSource& source, const TilePos &pos)
{
    Tile::onRemove(source, pos);
    source.removeTileEntity(pos);
}