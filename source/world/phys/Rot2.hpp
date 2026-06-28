#pragma once

struct Rot2
{
public:
	static const Rot2 ZERO;

public:
	float yaw;
	float pitch;

public:
	Rot2() : yaw(0), pitch(0) {}
	Rot2(float v) : yaw(v), pitch(v) {}
	Rot2(int v) : yaw(float(v)), pitch(float(v)) {}
	Rot2(float yaw, float pitch) : yaw(yaw), pitch(pitch) {}
	Rot2(int yaw, int pitch) : yaw(float(yaw)), pitch(float(pitch)) {}


	float getX() const { return pitch; }
	float getY() const { return yaw; }

	Rot2 translate(float y, float x) const
	{
		return Rot2(yaw + y, pitch + x);
	}

	// these are also likely inlined, but I'll declare them in the header
	Rot2 operator+(const Rot2& b) const
	{
		return Rot2(yaw + b.yaw, pitch + b.pitch);
	}

	Rot2 operator-(const Rot2& b) const
	{
		return Rot2(yaw - b.yaw, pitch - b.pitch);
	}

	Rot2 operator*(const Rot2& b) const
	{
		return Rot2(yaw * b.yaw, pitch * b.pitch);
	}

	void operator+=(const Rot2& b)
	{
		yaw += b.yaw;
		pitch += b.pitch;
	}

	void operator-=(const Rot2& b)
	{
		(*this) += -b;
	}

	void operator+=(float f)
	{
		yaw += f;
		pitch += f;
	}

	void operator-=(float f)
	{
		yaw -= f;
		pitch -= f;
	}

	void operator*=(float f)
	{
		yaw *= f;
		pitch *= f;
	}

	void operator/=(float f)
	{
		yaw /= f;
		pitch /= f;
	}

	Rot2 operator-() const
	{
		return Rot2(-yaw, -pitch);
	}

	Rot2 operator-(float f) const
	{
		return Rot2(yaw - f, pitch - f);
	}

	Rot2 operator*(float f) const
	{
		return Rot2(yaw * f, pitch * f);
	}

	Rot2 operator/(float f) const
	{
		return Rot2(yaw / f, pitch / f);
	}

	bool operator==(const Rot2& b) const
	{
		return yaw == b.yaw &&
			pitch == b.pitch;
	}

	bool operator!=(const Rot2& b) const
	{
		return yaw != b.yaw &&
			pitch != b.pitch;
	}
};
