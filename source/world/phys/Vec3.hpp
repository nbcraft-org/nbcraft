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

struct TilePos;

#define __VEC3_HPP

class Vec3
{
public:
	static const Vec3 ZERO, ONE;
	static const Vec3 UNIT_X, NEG_UNIT_X;
	static const Vec3 UNIT_Y, NEG_UNIT_Y;
	static const Vec3 UNIT_Z, NEG_UNIT_Z;
	static const Vec3 MIN, MAX;

public:
	float x, y, z;

public:
	// this constructor is nice to have, but it's probably inlined
	Vec3() : x(0), y(0), z(0) {}
	Vec3(float xyz) : x(xyz), y(xyz), z(xyz) {}
	Vec3(int xyz) : x(float(xyz)), y(float(xyz)), z(float(xyz)) {}
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vec3(int x, int y, int z) : x(float(x)), y(float(y)), z(float(z)) {}

	Vec3 interpolateTo(const Vec3& to, float t) const
	{
		return *this + (to - *this) * t;
	}

	Vec3 vectorTo(const Vec3& to) const
	{
		return Vec3(to.x - x, to.y - y, to.z - z);
	}

	Vec3 normalize() const
	{
		float dist = length();
		if (dist < 0.0001f)
			return ZERO;

		return Vec3(x / dist, y / dist, z / dist);
	}

	float dot(const Vec3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vec3 cross(const Vec3& other) const
	{
		return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	Vec3 add(float x, float y, float z) const
	{
		return Vec3(this->x + x, this->y + y, this->z + z); 
	}

	float distanceTo(const Vec3& b) const
	{ 
		return Mth::sqrt(distanceToSqr(b));
	}

	float distanceToSqr(const Vec3& b) const
	{
		return (*this - b).lengthSqr();
	}
    
	bool clipX(const Vec3& endPoint, float clipValue, Vec3& intersection) const
	{
		float deltaX = endPoint.x - this->x;
		if (deltaX * deltaX < 0.000001f)
			return false;

		float t = (clipValue - this->x) / deltaX;
		if (t < 0.0f || t > 1.0f)
			return false;

		intersection = interpolateTo(endPoint, t);
		return true;
	}

	bool clipY(const Vec3& endPoint, float clipValue, Vec3& intersection) const
	{
		float deltaY = endPoint.y - this->y;
		if (deltaY * deltaY < 0.000001f)
			return false;

		float t = (clipValue - this->y) / deltaY;
		if (t < 0.0f || t > 1.0f)
			return false;

		intersection = interpolateTo(endPoint, t);
		return true;
	}

	bool clipZ(const Vec3& endPoint, float clipValue, Vec3& intersection) const
	{
		float deltaZ = endPoint.z - this->z;
		if (deltaZ * deltaZ < 0.000001f)
			return false;

		float t = (clipValue - this->z) / deltaZ;
		if (t < 0.0f || t > 1.0f)
			return false;

		intersection = interpolateTo(endPoint, t);
		return true;
	}

	Vec3 translate(float tx, float ty, float tz) const
	{
		return Vec3(x + tx, y + ty, z + tz);
	}

	float lengthSqr() const
	{
		return x * x + y * y + z * z;
	}

	float length() const
	{
		return Mth::sqrt(lengthSqr());
	}

	Vec3 scale(float scale) const
	{
		return Vec3(x * scale, y * scale, z * scale);
	}

	Vec3 operator+(const Vec3& b) const
	{
		return Vec3(x + b.x, y + b.y, z + b.z);
	}

	Vec3 operator-(const Vec3& b) const
	{
		return Vec3(x - b.x, y - b.y, z - b.z);
	}

	Vec3 operator*(const Vec3& b) const
	{
		return Vec3(x * b.x, y * b.y, z * b.z);
	}

	void operator+=(const Vec3& b)
	{
		x += b.x;
		y += b.y;
		z += b.z;
	}

	void operator-=(const Vec3& b)
	{
		(*this) += -b;
	}

	void operator+=(float f)
	{
		x += f;
		y += f;
		z += f;
	}

	void operator-=(float f)
	{
		x -= f;
		y -= f;
		z -= f;
	}

	void operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
	}

	void operator/=(float f)
	{
		x /= f;
		y /= f;
		z /= f;
	}

	Vec3 operator-() const
	{
		return Vec3(-x, -y, -z);
	}

	Vec3 operator-(float f) const
	{
		return Vec3(x - f, y - f, z - f);
	}

	Vec3 operator*(float f) const
	{
		return Vec3(x * f, y * f, z * f);
	}

	Vec3 operator/(float f) const
	{
		return Vec3(x / f, y / f, z / f);
	}

	bool operator==(const Vec3& b) const
	{
		return x == b.x &&
			   y == b.y &&
			   z == b.z;
	}

	bool operator!=(const Vec3& b) const
	{
		return x != b.x &&
			   y != b.y &&
			   z != b.z;
	}
};

