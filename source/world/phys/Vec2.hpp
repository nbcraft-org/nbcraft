/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "common/Mth.hpp"
// Needed for when we're missing nullptr in multiple files
#include "compat/LegacyCPP.hpp"

class Vec2
{
public:
	static const Vec2 ZERO, ONE;
	static const Vec2 UNIT_X, NEG_UNIT_X;
	static const Vec2 UNIT_Y, NEG_UNIT_Y;
	static const Vec2 MIN, MAX;

public:
	float x, y;
    
private:
    void _init(float x, float y);

public:
	// this constructor is nice to have, but it's probably inlined
	Vec2() : x(0), y(0) {}
	Vec2(float xy) : x(xy), y(xy) {}
	Vec2(int xy) : x(float(xy)), y(float(xy)) {}
	Vec2(float x, float y) : x(x), y(y) {}
	Vec2(int x, int y) : x(float(x)), y(float(y)) {}
    
    Vec2 normalize() const
	{
		float dist = Mth::sqrt(x * x + y * y);
		if (dist < 0.0001f)
			return ZERO;

		return Vec2(x / dist, y / dist);
	}

	Vec2 translate(float tx, float ty) const
	{
		return Vec2(x + tx, y + ty);
	}

	float lengthSqr() const
	{
		return x * x + y * y;
	}

	float length() const
	{
		return Mth::sqrt(lengthSqr());
	}

	Vec2 scale(float scale) const
	{
		return Vec2(x * scale, y * scale);
	}

	// these are also likely inlined, but I'll declare them in the header
	Vec2 operator+(const Vec2& b) const
	{
		return Vec2(x + b.x, y + b.y);
	}

	Vec2 operator-(const Vec2& b) const
	{
		return Vec2(x - b.x, y - b.y);
	}

	Vec2 operator*(const Vec2& b) const
	{
		return Vec2(x * b.x, y * b.y);
	}

	void operator+=(const Vec2& b)
	{
		x += b.x;
		y += b.y;
	}

	void operator-=(const Vec2& b)
	{
		(*this) += -b;
	}

	void operator+=(float f)
	{
		x += f;
		y += f;
	}

	void operator-=(float f)
	{
		x -= f;
		y -= f;
	}

	void operator*=(float f)
	{
		x *= f;
		y *= f;
	}

	void operator/=(float f)
	{
		x /= f;
		y /= f;
	}

	Vec2 operator-() const
	{
		return Vec2(-x, -y);
	}

	Vec2 operator-(float f) const
	{
		return Vec2(x - f, y - f);
	}

	Vec2 operator*(float f) const
	{
		return Vec2(x * f, y * f);
	}

	Vec2 operator/(float f) const
	{
		return Vec2(x / f, y / f);
	}

	bool operator==(const Vec2& b) const
	{
		return x == b.x &&
			y == b.y;
	}

	bool operator!=(const Vec2& b) const
	{
		return x != b.x &&
			y != b.y;
	}
};

