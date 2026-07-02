#pragma once

struct TilePos;

class Facing
{
public:
	enum Name
	{
		DOWN,  // -Y
		UP,    // +Y
		NORTH, // -Z
		SOUTH, // +Z
		WEST,  // -X
		EAST,  // +X
		COUNT
	};

public:
	static const Name DIRECTIONS[COUNT];
	static const TilePos DIRECTION[COUNT];
	static const Name OPPOSITE[6];
	static const Name HORIZONTAL[4];
};