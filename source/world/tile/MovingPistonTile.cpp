#include "MovingPistonTile.hpp"
#include "world/level/Level.hpp"
#include "entity/PistonMovingTileEntity.hpp"

MovingPistonTile::MovingPistonTile(TileID id) : EntityTile(id, 0, Material::piston)
{
    setDestroyTime(-1.0f);
}

void MovingPistonTile::onPlace(Level*, const TilePos& pos)
{
}

void MovingPistonTile::onRemove(Level* level, const TilePos& pos)
{
    PistonMovingTileEntity* piston = static_cast<PistonMovingTileEntity*>(level->getTileEntity(pos));
    if (piston)
        piston->finalTick();
    else
        Tile::onRemove(level, pos);
}

bool MovingPistonTile::mayPlace(const Level*, const TilePos& pos) const
{
    return false;
}

//bool MovingPistonTile::mayPlace(const Level*, const TilePos& pos, Facing::Name face) const
//{
//    return false;
//}

eRenderShape MovingPistonTile::getRenderShape() const
{
    return SHAPE_NONE;
}

bool MovingPistonTile::isSolidRender() const
{
    return false;
}

bool MovingPistonTile::isCubeShaped() const
{
    return false;
}

bool MovingPistonTile::use(Level* level, const TilePos& pos, Player* player)
{
    if (level->m_bIsClientSide && !level->getTileEntity(pos))
    {
        level->setTile(pos, TILE_AIR);
        return true;
    }
    else
        return false;
}

int MovingPistonTile::getResource(TileData, Random*)
{
    return 0;
}

void MovingPistonTile::spawnResources(Level* level, const TilePos& pos, TileData, float)
{
    if (!level->m_bIsClientSide)
    {
        PistonMovingTileEntity* piston = static_cast<PistonMovingTileEntity*>(level->getTileEntity(pos));
        if (piston)
            Tile::tiles[piston->getTileId()]->spawnResources(level, pos, piston->getData());
    }
}

AABB* MovingPistonTile::getAABB(const Level* level, const TilePos& pos)
{
    PistonMovingTileEntity* piston = static_cast<PistonMovingTileEntity*>(level->getTileEntity(pos));
    if (piston)
    {
        float progress = piston->getProgress(0.0f);
        if (piston->isExtending())
            progress = 1.0f - progress;

        return moveByPositionAndProgress(level, pos, piston->getTileId(), progress, piston->getDirection());
    }
    return nullptr;
}

void MovingPistonTile::updateShape(const LevelSource* level, const TilePos& pos)
{
    PistonMovingTileEntity* piston = static_cast<PistonMovingTileEntity*>(level->getTileEntity(pos));
    if (piston) {
        Tile* tile = Tile::tiles[piston->getTileId()];
        if (!tile || tile == this)
            return;

        tile->updateShape(level, pos);
        float progress = piston->getProgress(0.0f);
        if (piston->isExtending())
            progress = 1.0f - progress;

        m_aabb.move(Vec3(TilePos::ZERO.relative(piston->getDirection())) * -progress);
    }
}

TileEntity* MovingPistonTile::newTileEntity()
{
    return nullptr;
}

AABB* MovingPistonTile::moveByPositionAndProgress(const Level* level, const TilePos& pos, TileID id, float progress, Facing::Name dir)
{
    if (id && id != m_ID)
    {
        AABB* aabb = Tile::tiles[id]->getAABB(level, pos);
        if (!aabb)
            return nullptr;
        else
        {
            aabb->move(Vec3(TilePos::ZERO.relative(dir)) * -progress);
            return aabb;
        }
    }
    else
        return nullptr;
}

PistonMovingTileEntity* MovingPistonTile::newMovingTileEntity(TileID tile, int meta, Facing::Name direction, bool extending, bool isSourcePiston)
{
    return new PistonMovingTileEntity(tile, meta, direction, extending, isSourcePiston);
}
