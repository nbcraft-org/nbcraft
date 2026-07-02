#include "Facing.hpp"

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

const int Facing::NORMALS[6][3] = {
	{0, -1,  0},
	{0,  1,  0},
	{0,  0, -1},
	{0,  0,  1},
	{-1,  0,  0},
	{1,  0,  0}
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
	{0, 1, 2, 3}, // 0�
	{1, 2, 3, 0}, // 90�
	{3, 0, 1, 2}, // 270�
	{2, 3, 0, 1}  // 180�
};

const float Facing::LIGHT[] = { 0.5f, 1.0f, 0.8f, 0.8f, 0.6f, 0.6f };