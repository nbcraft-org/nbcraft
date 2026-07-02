#include "MobSpawnerTile.hpp"
#include "world/level/Level.hpp"
#include "entity/MobSpawnerTileEntity.hpp"

MobSpawnerTile::MobSpawnerTile(TileID id, int texture) 
    : EntityTile(id, texture, Material::stone)
{
    m_renderLayer = RENDER_LAYER_ALPHATEST;
}

TileEntity* MobSpawnerTile::newTileEntity()
{
    return new MobSpawnerTileEntity;
}

int MobSpawnerTile::getResource(TileData, Random*) const 
{
    return 0;
}

int MobSpawnerTile::getResourceCount(Random*) const
{
     return 0;
}

bool MobSpawnerTile::isSolidRender() const
{
    return false;
}