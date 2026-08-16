#pragma once

#include "Tile.hpp"
#include "world/level/Level.hpp"

#define C_POWERED_BIT 8

class RailTile : public Tile
{
public:
    enum Face
    {
        NORTH_SOUTH = 0,
        WEST_EAST = 1,
        WEST_EAST_ABOVE = 2,
        EAST_WEST_ABOVE = 3,
        SOUTH_NORTH_ABOVE = 4,
        NORTH_SOUTH_ABOVE = 5,
        EAST_SOUTH = 6,
        WEST_SOUTH = 7,
        WEST_NORTH = 8,
        EAST_NORTH = 9
    };

    RailTile(TileID id, int texture, bool isPowered);

private:
    void _updateDir(TileSource&, const TilePos& pos, bool updateNeighbors);
    bool _applyPower(TileSource&, const TilePos& pos, TileData data, bool isForward, int powerDistance);
    bool _canPower(TileSource&, const TilePos& pos, bool isForward, int powerDistance, int var7);
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

    inline TileData getFaceData(TileData data) const { return _m_bIsPowered ? (data & (C_POWERED_BIT - 1)) : data; }
    inline bool getPowered(TileData data) const { return (_m_bIsPowered ? (data & C_POWERED_BIT) : 0) != 0; }

    static bool hasRail(TileSource& level, const TilePos& pos)
    {
        return isRail(level.getTile(pos));
    }

    static bool isRail(TileID id)
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
        return rail && rail->_m_bIsPowered;
    }

private:
    bool _m_bIsPowered;
};