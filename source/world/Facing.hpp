#pragma once

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
		EAST   // +X
	};
	static const Name OPPOSITE[6];
	static const Name HORIZONTAL[4];
	static const int CORNERS[8][3];
	static const int VERTICES[6][4];
	static const int NORMALS[6][3];
	static const int UVS[6][4][2];
	static const int UV_AXES[3][4];
	static const int ROTATED_INDEX[4][4];
	static const float LIGHT[6];

	static bool isVertical(Name facing)
	{
		return facing == DOWN || facing == UP;
	}

	static bool isHorizontal(Name facing)
	{
		return !isVertical(facing);
	}
};