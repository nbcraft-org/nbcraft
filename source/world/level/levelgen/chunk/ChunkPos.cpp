#include "ChunkPos.hpp"
#include "world/level/TilePos.hpp"
#include "world/phys/Vec3.hpp"

void ChunkPos::_init(const Vec3& pos)
{
	_init(TilePos(pos));
}

void ChunkPos::_init(const TilePos& pos)
{
	x = ToChunkCoordinate(pos.x);
	z = ToChunkCoordinate(pos.z);
}

ChunkPos::operator TilePos() const
{
	return TilePos(*this, 0);
}
