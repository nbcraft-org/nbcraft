#pragma once

#include "Tile.hpp"
#include "world/level/Level.hpp"

class RailTile : public Tile
{
public:
    RailTile(TileID id, int texture, bool isPowered);
    bool mayPlace(const Level*, const TilePos& pos) const override;
    void neighborChanged(Level* level, const TilePos& pos, TileID tile) override;
    bool isSolidRender() const override;
    bool isCubeShaped() const override;
    AABB* getAABB(const Level*, const TilePos& pos) override;
    eRenderShape getRenderShape() const override;
    int getTexture(Facing::Name face, TileData data) const override;
    void updateShape(const LevelSource* level, const TilePos& pos) override;
    void setPlacedBy(Level*, const TilePos& pos, Mob*) override;
    void onPlace(Level*, const TilePos& pos) override;

    static bool hasRail(Level* level, const TilePos& pos)
    {
        return isRail(level->getTile(pos));
    }

    static bool isRail(int id)
    {
        return id == Tile::rail->m_ID || id == Tile::poweredRail->m_ID || id == Tile::detectorRail->m_ID;
    }

    static bool isPoweredRail(const Tile* tile)
    {
        return tile && tile->m_ID == Tile::poweredRail->m_ID;
    }

    static bool isPowered(const Tile* tile)
    {
        if (!tile) return false;
        RailTile* rail = (RailTile*)tile;
        return rail && rail->m_bIsPowered;
    }

    bool m_bIsPowered;

private:
    void updateDir(Level* level, const TilePos& pos, bool updateNeighbors);
    bool applyPower(Level* level, const TilePos& pos, TileData data, bool var6, int var7);
    bool canPower(Level* level, const TilePos& pos, bool var5, int var6, int var7);

};
