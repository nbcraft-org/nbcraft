#include <limits.h>

#include "TilePos.hpp"
#include "world/phys/Vec3.hpp"
#include "world/level/levelgen/chunk/ChunkPos.hpp"

const TilePos TilePos::ZERO = TilePos(0, 0, 0);
const TilePos TilePos::ONE = TilePos(1, 1, 1);
const TilePos TilePos::MIN = TilePos(INT_MIN, INT_MIN, INT_MIN);
const TilePos TilePos::MAX = TilePos(INT_MAX, INT_MAX, INT_MAX);

TilePos::TilePos(const ChunkPos& pos, int _y)
{
    x = FromChunkCoordinate(pos.x);
    y = _y;
    z = FromChunkCoordinate(pos.z);
}

