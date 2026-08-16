#pragma once

#include "Tile.hpp"
#include "world/level/Level.hpp"

#define C_POWERED_BIT 8

class RailTile : public Tile
{
public:
    RailTile(TileID id, int texture, bool isPowered);

private:
    void _updateDir(TileSource&, const TilePos& pos, bool updateNeighbors);
    bool _applyPower(TileSource&, const TilePos& pos, TileData data, bool isForward, int powerDistance);
    bool _canPower(TileSource&, const TilePos& pos, bool isForward, int powerDistance, int var7);
    inline TileData _getFaceData(TileData data) const { return _m_bIsPowered ? (data & (C_POWERED_BIT - 1)) : data; }
    inline TileData _getPowered(TileData data) const { return _m_bIsPowered ? (data & C_POWERED_BIT) : 0; }
	bool _updatePower(TileSource&, const TilePos& pos, TileData data);

public:
    bool mayPlace(const TileSource&, const TilePos& pos) const override;
    void neighborChanged(TileSource&, const TilePos& pos, TileID tile) override;
    bool isSolidRender() const override;
    bool isCubeShaped() const override;
    AABB* getAABB(const TileSource&, const TilePos& pos) override;
    eRenderShape getRenderShape() const override;
    int getTexture(Facing::Name face, TileData data) const override;
    void updateShape(const TileSource&, const TilePos& pos) override;
    void setPlacedBy(const TilePos& pos, Mob&) override;
    void onPlace(TileSource&, const TilePos& pos) override;

    static bool hasRail(TileSource& level, const TilePos& pos)
    {
        return isRail(level.getTile(pos));
    }

    static bool isRail(int id)
    {
        return id == Tile::rail->m_ID || id == Tile::poweredRail->m_ID || id == Tile::detectorRail->m_ID;
    }

    static bool isPoweredRail(const Tile* tile)
    {
        return tile && tile->m_ID == Tile::poweredRail->m_ID;
    }

    // same as isPoweredRail ?
    static bool isPowered(const Tile* tile)
    {
        if (!tile) return false;
        RailTile* rail = (RailTile*)tile;
        return rail && rail->_m_bIsPowered;
    }

private:
    bool _m_bIsPowered;
};