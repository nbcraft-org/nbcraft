#include "Facing.hpp"
#include "world/level/TilePos.hpp"

const Facing::Name Facing::DIRECTIONS[COUNT] = {
	Facing::DOWN,
	Facing::UP,
	Facing::NORTH,
	Facing::SOUTH,
	Facing::WEST,
	Facing::EAST
};

const TilePos Facing::DIRECTION[COUNT] = {
	TilePos(0, -1,  0), // DOWN
	TilePos(0,  1,  0), // UP
	TilePos(0,  0, -1), // NORTH
	TilePos(0,  0,  1), // SOUTH
	TilePos(-1, 0,  0), // WEST
	TilePos(1,  0,  0)  // EAST
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

const Facing::Name Facing::HORIZONTAL[4] =
{
    Facing::NORTH,
    Facing::SOUTH,
    Facing::EAST,
    Facing::WEST
};

const int Facing::CORNERS[8][3] = {
	{0, 1, 2},
	{3, 1, 2},
	{3, 4, 2},
	{0, 4, 2},
	{0, 1, 5},
	{3, 1, 5},
	{3, 4, 5},
	{0, 4, 5},
};

const int Facing::VERTICES[6][4] = {
	{4, 0, 1, 5},
	{6, 2, 3, 7},
	{3, 2, 1, 0},
	{7, 4, 5, 6},
	{7, 3, 0, 4},
	{5, 1, 2, 6}
};

const Vec3 Facing::NORMALS[6] = {
	Vec3(0, -1, 0), // DOWN
	Vec3(0, 1, 0),  // UP
	Vec3(0, 0, -1), // NORTH
	Vec3(0, 0, 1),  // SOUTH
	Vec3(-1, 0, 0), // WEST
	Vec3(1, 0, 0)   // EAST
};

const int Facing::UVS[6][4][2] = {
	{ {0, 1}, {0, 0}, {1, 0}, {1, 1} },
	{ {1, 1}, {1, 0}, {0, 0}, {0, 1} },
	{ {1, 0}, {0, 0}, {0, 1}, {1, 1} },
	{ {0, 0}, {0, 1}, {1, 1}, {1, 0} },
	{ {1, 0}, {0, 0}, {0, 1}, {1, 1} },
	{ {0, 1}, {1, 1}, {1, 0}, {0, 0} }
};

const int Facing::UV_AXES[3][4] = {
	{0, 3, 2, 5},
	{0, 3, 1, 4},
	{2, 5, 1, 4}
};

const int Facing::ROTATED_INDEX[4][4] = {
	{0, 1, 2, 3}, // 0 deg
	{1, 2, 3, 0}, // 90 deg
	{3, 0, 1, 2}, // 270 deg
	{2, 3, 0, 1}  // 180 deg
};

const float Facing::LIGHT[] = { 0.5f, 1.0f, 0.8f, 0.8f, 0.6f, 0.6f };
