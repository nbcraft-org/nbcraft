#pragma once

#include "RailTile.hpp"

class Rail
{
private:
    Level* m_pLevel;
    TilePos m_pos;
    bool m_bPowered;
    std::vector<TilePos> m_connections;

public:
    Rail(Level* level, TilePos pos);

    void updateConnections(TileData data);
    void removeSoftConnections();
    bool hasRail(const TilePos& p) { return RailTile::hasRail(m_pLevel, p) || RailTile::hasRail(m_pLevel, p.above()) || RailTile::hasRail(m_pLevel, p.below()); }
    Rail* getRail(const TilePos& p);
    bool connectsTo(const Rail& other);
    bool hasConnection(const TilePos& tp);
    int countPotentialConnections();
    bool canConnectTo(Rail& other);
    void connectTo(Rail& other);
    bool hasNeighborRail(const TilePos& p);
    void place(bool hasSignal, bool checkNeighbors);
};
