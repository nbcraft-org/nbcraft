#include "PortalTile.hpp"
#include "world/level/Level.hpp"

PortalTile::PortalTile(TileID ID, int texture) : HalfTransparentTile(ID, texture, Material::portal)
{
    m_renderLayer = RENDER_LAYER_BLEND;
}

AABB* PortalTile::getAABB(const Level*, const TilePos& pos)
{
	return nullptr;
}

void PortalTile::updateShape(const LevelSource* level, const TilePos& pos)
{
    float var5;
    float var6;
    if (level->getTile(pos.west()) != m_ID && level->getTile(pos.east()) != m_ID)
    {
        var5 = 0.125f;
        var6 = 0.5f;
    }
    else
    {
        var5 = 0.5f;
        var6 = 0.125f;
    }
    setShape(0.5f - var5, 0.0f, 0.5f - var6, 0.5f + var5, 1.0f, 0.5f + var6);
}

bool PortalTile::isCubeShaped() const
{
	return false;
}

bool PortalTile::trySpawnPortal(Level* level, const TilePos& pos)
{
    int xDiff = 0;
    int zDiff = 0;
    if (level->getTile(pos.west()) == Tile::obsidian->m_ID || level->getTile(pos.east()) == Tile::obsidian->m_ID)
        xDiff = 1;

    if ((level->getTile(pos.north()) == Tile::obsidian->m_ID || level->getTile(pos.south()) == Tile::obsidian->m_ID))
        zDiff = 1;

    if (xDiff == zDiff)
        return false;
    else
    {
        TilePos tp(pos);
        if (level->isEmptyTile(pos + TilePos(-xDiff, 0, -zDiff)))
        {
            tp.x -= xDiff;
            tp.z -= zDiff;
        }

        int var7;
        int var8;
        for (var7 = -1; var7 <= 2; ++var7) {
            for (var8 = -1; var8 <= 3; ++var8) {
                bool var9 = var7 == -1 || var7 == 2 || var8 == -1 || var8 == 3;
                if (var7 != -1 && var7 != 2 || var8 != -1 && var8 != 3) {
                    int var10 = level->getTile(tp + TilePos(xDiff * var7, var8, zDiff * var7));
                    if (var9)
                    {
                        if (var10 != Tile::obsidian->m_ID)
                            return false;
                    }
                    else if (var10 != 0 && var10 != Tile::fire->m_ID)
                        return false;
                }
            }
        }

        level->m_bNoNeighborUpdate = true;

        for (var7 = 0; var7 < 2; ++var7) {
            for (var8 = 0; var8 < 3; ++var8) {
                level->setTile(tp + TilePos(xDiff * var7, var8, zDiff * var7), Tile::portal->m_ID);
            }
        }

        level->m_bNoNeighborUpdate = false;
        return true;
    }
}

int PortalTile::getResourceCount(Random* pRandom) const
{
	return 0;
}

void PortalTile::neighborChanged(Level* level, const TilePos& pos, TileID newTile)
{
    uint8_t xDiff = 0;
    uint8_t zDiff = 1;
    if (level->getTile(pos.west()) == m_ID || level->getTile(pos.east()) == m_ID)
    {
        xDiff = 1;
        zDiff = 0;
    }

    TilePos tp(pos);
    for (; level->getTile(tp.below()) == m_ID; --tp.y) {
    }

    if (level->getTile(tp.below()) != Tile::obsidian->m_ID)
        level->setTile(pos, 0);
    else
    {
        int var9;
        for (var9 = 1; var9 < 4 && level->getTile(tp.above(var9)) == m_ID; ++var9) {
        }

        if (var9 == 3 && level->getTile(tp.above(var9)) == Tile::obsidian->m_ID)
        {
            bool var10 = level->getTile(pos.west()) == m_ID || level->getTile(pos.east()) == m_ID;
            bool var11 = level->getTile(pos.north()) == m_ID || level->getTile(pos.south()) == m_ID;
            if (var10 && var11)
                level->setTile(pos, TILE_AIR);
            else if ((level->getTile(pos + TilePos(xDiff, 0, zDiff)) != Tile::obsidian->m_ID || level->getTile(pos + TilePos(-xDiff, 0, -zDiff)) != m_ID) && (level->getTile(pos + TilePos(-xDiff, 0, -zDiff)) != Tile::obsidian->m_ID || level->getTile(pos + TilePos(xDiff, 0, zDiff)) != m_ID))
                level->setTile(pos, TILE_AIR);

        }
        else
            level->setTile(pos, TILE_AIR);
    }
}

void PortalTile::entityInside(Level* level, const TilePos& pos, Entity* entity) const
{
    if (!entity->getRider() && !entity->getRiding())
        entity->handleInsidePortal();
}

bool PortalTile::shouldRenderFace(const LevelSource* level, const TilePos& pos, Facing::Name face) const
{
    if (level->getTile(pos) == m_ID)
        return false;
    else
    {
        bool w = level->getTile(pos.west()) == m_ID && level->getTile(pos.west(2)) != m_ID;
        bool e = level->getTile(pos.east()) == m_ID && level->getTile(pos.east(2)) != m_ID;
        bool n = level->getTile(pos.north()) == m_ID && level->getTile(pos.north(2)) != m_ID;
        bool s = level->getTile(pos.south()) == m_ID && level->getTile(pos.south(2)) != m_ID;
        bool we = w || e;
        bool ns = n || s;
        return (we && (face == Facing::WEST || face == Facing::EAST)) || (ns && (face == Facing::NORTH || face == Facing::SOUTH));
    }
}

void PortalTile::animateTick(Level* level, const TilePos& pos, Random* random)
{
    if (random->nextInt(100) == 0)
        level->playSound(Vec3(pos) + 0.5f, "portal.portal", 1.0f, random->nextFloat() * 0.4f + 0.8f);

    for (int var6 = 0; var6 < 4; ++var6)
    {
        float var7 = pos.x + random->nextFloat();
        float var9 = pos.y + random->nextFloat();
        float var11 = pos.z + random->nextFloat();
        Vec3 vel;
        int var19 = random->nextInt(2) * 2 - 1;
        vel.x = (random->nextFloat() - 0.5) * 0.5;
        vel.y = (random->nextFloat() - 0.5) * 0.5;
        vel.z = (random->nextFloat() - 0.5) * 0.5;
        if (level->getTile(pos.west()) != m_ID && level->getTile(pos.east()) != m_ID) {
            var7 = pos.x + 0.5 + 0.25 * var19;
            vel.x = (random->nextFloat() * 2.0f * var19);
        }
        else {
            var11 = pos.z + 0.5 + 0.25 * var19;
            vel.z = (random->nextFloat() * 2.0f * var19);
        }

        level->addParticle("portal", Vec3(var7, var9, var11), vel);
    }
}
