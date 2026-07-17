#include "Rail.hpp"

Rail::Rail(Level* level, TilePos pos)
    : m_pLevel(level)
    , m_pos(pos)
{
    TileData data = level->getData(pos);
    if (RailTile::isPowered(Tile::tiles[level->getTile(pos)])) {
        m_bPowered = true;
        data &= -9;
    }
    else
        m_bPowered = false;
 
    updateConnections(data);
}

void Rail::removeSoftConnections()
{
    for (size_t i = 0; i < m_connections.size(); ++i)
    {
        Rail* other = getRail(m_connections[i]);
        if (other && other->connectsTo(*this)) {
            m_connections[i] = other->m_pos;
        }
        else {
            m_connections.erase(m_connections.begin() + i);
            --i;
        }
        SAFE_DELETE(other);
    }
}

Rail* Rail::getRail(const TilePos& p)
{
    if (RailTile::hasRail(m_pLevel, p))
        return new Rail(m_pLevel, p);

    TilePos above = p.above();
    if (RailTile::hasRail(m_pLevel, above))
        return new Rail(m_pLevel, above);

    TilePos below = p.below();
    if (RailTile::hasRail(m_pLevel, below))
        return new Rail(m_pLevel, below);

    return nullptr;
}

bool Rail::connectsTo(const Rail& other)
{
    for (std::vector<TilePos>::iterator it = m_connections.begin(); it != m_connections.end(); ++it)
    {
        const TilePos& conn = *it;
        if (conn.x == other.m_pos.x && conn.z == other.m_pos.z)
            return true;
    }
    return false;
}

bool Rail::hasConnection(const TilePos& tp)
{
    for (std::vector<TilePos>::iterator it = m_connections.begin(); it != m_connections.end(); ++it)
    {
        const TilePos& conn = *it;
        if (conn.x == tp.x && conn.z == tp.z)
            return true;
    }
    return false;
}

int Rail::countPotentialConnections()
{
    int count = 0;
    for (int face = 0; face < 4; ++face)
        if (hasRail(m_pos.relative(Facing::HORIZONTAL[face]))) ++count;

    return count;
}

bool Rail::canConnectTo(Rail& other)
{
    if (connectsTo(other)) return true;
    if (m_connections.size() == 2) return false;
    if (m_connections.empty()) return true;

    TilePos first = m_connections[0];
    return other.m_pos.y == m_pos.y && first.y == m_pos.y;
}

void Rail::connectTo(Rail& other)
{
    m_connections.push_back(other.m_pos);

    bool n = hasConnection(m_pos.north());
    bool s = hasConnection(m_pos.south());
    bool w = hasConnection(m_pos.west());
    bool e = hasConnection(m_pos.east());

    int newData = -1;

    if ((n || s) && !w && !e) newData = 0;
    if ((w || e) && !n && !s) newData = 1;

    if (!m_bPowered)
    {
        if (s && e && !n && !w) newData = 6;
        if (s && w && !n && !e) newData = 7;
        if (n && w && !s && !e) newData = 8;
        if (n && e && !s && !w) newData = 9;
    }

    if (newData == 0)
    {
        if (RailTile::hasRail(m_pLevel, TilePos(m_pos.x, m_pos.y + 1, m_pos.z - 1))) newData = 4;
        if (RailTile::hasRail(m_pLevel, TilePos(m_pos.x, m_pos.y + 1, m_pos.z + 1))) newData = 5;
    }

    if (newData == 1)
    {
        if (RailTile::hasRail(m_pLevel, TilePos(m_pos.x + 1, m_pos.y + 1, m_pos.z))) newData = 2;
        if (RailTile::hasRail(m_pLevel, TilePos(m_pos.x - 1, m_pos.y + 1, m_pos.z))) newData = 3;
    }

    if (newData < 0)
        newData = !RailTile::isRail(m_pLevel->getTile(m_pos)) ? 0 : m_pLevel->getData(m_pos);

    if (m_bPowered)
        newData = (m_pLevel->getData(m_pos) & 8) | newData;

    m_pLevel->setData(m_pos, newData);
}

bool Rail::hasNeighborRail(const TilePos& p)
{
    Rail* neighbor = getRail(p);
    if (!neighbor) return false;
    neighbor->removeSoftConnections();
    bool canConnect = neighbor->canConnectTo(*this);
    delete neighbor;
    return canConnect;
}

void Rail::place(bool hasSignal, bool checkNeighbors)
{
    bool n = hasNeighborRail(m_pos.north());
    bool s = hasNeighborRail(m_pos.south());
    bool w = hasNeighborRail(m_pos.west());
    bool e = hasNeighborRail(m_pos.east());

    int newData = -1;

    if ((n || s) && !w && !e) newData = 0;
    if ((w || e) && !n && !s) newData = 1;

    if (!m_bPowered)
    {
        if (s && e && !n && !w) newData = 6;
        if (s && w && !n && !e) newData = 7;
        if (n && w && !s && !e) newData = 8;
        if (n && e && !s && !w) newData = 9;
    }

    if (newData == -1)
    {
        if (n || s) newData = 0;
        if (w || e) newData = 1;

        if (!m_bPowered)
        {
            if (hasSignal)
            {
                if (s && e) newData = 6;
                if (w && s) newData = 7;
                if (e && n) newData = 9;
                if (n && w) newData = 8;
            }
            else
            {
                if (n && w) newData = 8;
                if (e && n) newData = 9;
                if (w && s) newData = 7;
                if (s && e) newData = 6;

            }
        }
    }

    if (newData == 0)
    {
        if (RailTile::hasRail(m_pLevel, TilePos(m_pos.x, m_pos.y + 1, m_pos.z - 1))) newData = 4;
        if (RailTile::hasRail(m_pLevel, TilePos(m_pos.x, m_pos.y + 1, m_pos.z + 1))) newData = 5;
    }

    if (newData == 1)
    {
        if (RailTile::hasRail(m_pLevel, TilePos(m_pos.x + 1, m_pos.y + 1, m_pos.z))) newData = 2;
        if (RailTile::hasRail(m_pLevel, TilePos(m_pos.x - 1, m_pos.y + 1, m_pos.z))) newData = 3;
    }

    if (newData < 0)
        newData = !RailTile::isRail(m_pLevel->getTile(m_pos)) ? 0 : m_pLevel->getData(m_pos);

    updateConnections(newData);

    if (m_bPowered)
        newData = (m_pLevel->getData(m_pos) & 8) | newData;

    if (checkNeighbors || m_pLevel->getData(m_pos) != newData)
    {
        m_pLevel->setData(m_pos, newData);
        for (std::vector<TilePos>::iterator it = m_connections.begin(); it != m_connections.end(); ++it)
        {
            const TilePos& conn = *it;
            Rail* neighbor = getRail(conn);
            if (neighbor)
            {
                neighbor->removeSoftConnections();
                if (neighbor->canConnectTo(*this))
                    neighbor->connectTo(*this);

                delete neighbor;
            }
        }
    }
}

void Rail::updateConnections(TileData data)
{
    m_connections.clear();

    switch (data)
    {
    case 0:
        m_connections.push_back(m_pos.north());
        m_connections.push_back(m_pos.south());
        break;
    case 1:
        m_connections.push_back(m_pos.west());
        m_connections.push_back(m_pos.east());
        break;
    case 2:
        m_connections.push_back(m_pos.west());
        m_connections.push_back(m_pos.east().above());
        break;
    case 3:
        m_connections.push_back(m_pos.west().above());
        m_connections.push_back(m_pos.east());
        break;
    case 4:
        m_connections.push_back(m_pos.north().above());
        m_connections.push_back(m_pos.south());
        break;
    case 5:
        m_connections.push_back(m_pos.north());
        m_connections.push_back(m_pos.south().above());
        break;
    case 6:
        m_connections.push_back(m_pos.east());
        m_connections.push_back(m_pos.south());
        break;
    case 7:
        m_connections.push_back(m_pos.west());
        m_connections.push_back(m_pos.south());
        break;
    case 8:
        m_connections.push_back(m_pos.west());
        m_connections.push_back(m_pos.north());
        break;
    case 9:
        m_connections.push_back(m_pos.east());
        m_connections.push_back(m_pos.north());
        break;
    }
}
