#include "Facing.hpp"
#include "world/level/TilePos.hpp"

Facing::Name Facing::DIRECTIONS[COUNT] = {
	Facing::DOWN,
	Facing::UP,
	Facing::NORTH,
	Facing::SOUTH,
	Facing::WEST,
	Facing::EAST
};

const Facing::Name Facing::OPPOSITE[6] =
{
    Facing::UP,    // DOWN -> UP
    Facing::DOWN,  // UP -> DOWN
    Facing::SOUTH, // NORTH -> SOUTH
    Facing::NORTH, // SOUTH -> NORTH
    Facing::EAST,  // WEST -> EAST
    Facing::WEST   // EAST -> WEST
};

TilePos Facing::DIRECTION[COUNT] = {
	TilePos(0, -1,  0), // DOWN
	TilePos(0,  1,  0), // UP
	TilePos(0,  0, -1), // NORTH
	TilePos(0,  0,  1), // SOUTH
	TilePos(-1, 0,  0), // WEST
	TilePos(1,  0,  0)  // EAST
};

const Facing::Name Facing::HORIZONTAL[4] =
{
    Facing::NORTH,
    Facing::SOUTH,
    Facing::EAST,
    Facing::WEST
};