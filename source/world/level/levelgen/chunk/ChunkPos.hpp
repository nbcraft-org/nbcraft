#pragma once
#include <cmath>
#include "world/phys/Vec3.hpp"

#ifndef __TILEPOS_HPP
struct TilePos;
#endif

#define __CHUNKPOS_HPP

struct ChunkPos
{
public:
	int x, z;

private:
	void _init(const Vec3& pos);
	void _init(const TilePos& pos);
    
public:
	ChunkPos() : x(0), z(0) {}
	ChunkPos(int x, int z) : x(x), z(z) {}
	ChunkPos(const Vec3& pos) { _init(pos); }
	ChunkPos(const TilePos& pos) { _init(pos); }

	int lengthSqr() const
	{
		return x * x + z * z;
	}

	bool operator<(const ChunkPos& b) const
	{
		if (x != b.x)
			return x < b.x;

		return z < b.z;
	}

	bool operator>(const ChunkPos& b) const
	{
		if (x != b.x)
			return x > b.x;

		return z > b.z;
	}

	bool operator<=(const ChunkPos& b) const
	{
		return *this < b || *this == b;
	}

	bool operator>=(const ChunkPos& b) const
	{
		return *this > b || *this == b;
	}

	ChunkPos operator+(const ChunkPos& b) const
	{
		return ChunkPos(x + b.x, z + b.z);
	}

	ChunkPos operator+(int i) const
	{
		return ChunkPos(x + i, z + i);
	}

	ChunkPos operator-(const ChunkPos& b) const
	{
		return ChunkPos(x - b.x, z - b.z);
	}

	ChunkPos operator-(int i) const
	{
		return *this - ChunkPos(i, i);
	}

	void operator+=(const ChunkPos& b)
	{
		x += b.x;
		z += b.z;
	}

	void operator-=(const ChunkPos& b)
	{
		(*this) += -b;
	}

	void operator*=(int i)
	{
		x *= i;
		z *= i;
	}

	ChunkPos operator-() const
	{
		return ChunkPos(-x, -z);
	}

	ChunkPos operator*(int i) const
	{
		return ChunkPos(x * i, z * i);
	}

	ChunkPos operator/(int i) const
	{
		return ChunkPos(x / i, z / i);
	}

	bool operator==(const ChunkPos& other) const
	{
		return x == other.x &&
			z == other.z;
	}

	bool operator!=(const ChunkPos& other) const
	{
		return !(*this == other);
	}

	operator TilePos() const;

	static int ToChunkCoordinate(int value)
	{
		return value >> 4;
	}

	static int ToChunkCoordinate(float value)
	{
		return int(floorf(value / 16));
	}
};
