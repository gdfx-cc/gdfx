//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_MATH_VECTOR2_HPP__
#define __GDFX_MATH_VECTOR2_HPP__

#include <cmath>
#include <gdfx/math/Math.hpp>

namespace gdfx {

/**
 * Vector2
 */
class Vector2 {
public:
	float x;
	float y;

	Vector2() : x(0), y(0) {}
	Vector2(const Vector2& v) : x(v.x), y(v.y) {}
	Vector2(float nx, float ny) : x(nx), y(ny) {}

	Vector2& set(float nx, float ny) 
	{ 
		x = nx; y = ny;
		return *this;
	}

	Vector2& set(const Vector2& v) { return set(v.x, v.y); }
	Vector2& zero() { x = 0; y = 0; return *this; }
	float getLength() const { return std::sqrt(x*x + y*y); }

	Vector2& normalize()
	{
		float lengthSquared = x*x + y*y;
		if (lengthSquared > 0) {
			float inv = 1.0f / std::sqrt(lengthSquared);
			x *= inv;
			y *= inv;
		}
		return *this;
	}

	Vector2 getNormalized() const { Vector2 v(*this); return v.normalize(); }

	Vector2& rotate(const Vector2& pivot, float angleInDegrees)
	{
		float s = sin(Math::degreesToRadians(angleInDegrees));
		float c = cos(Math::degreesToRadians(angleInDegrees));

		x -= pivot.x;
		y -= pivot.y;

		float xNew = x * c - y * s;
		float yNew = x * s - y * c;

		x = xNew + pivot.x;
		y = yNew + pivot.y;

		return *this;
	}
	
	Vector2 getRotated(const Vector2& pivot, float angleInDegrees) const
	{
		Vector2 v(*this);
		return v.rotate(pivot, angleInDegrees);
	}

	Vector2& operator=(const Vector2& v) { return set(v); }

	bool operator==(const Vector2& v) const 
	{ 
		return x == v.x && y == v.y; 
	}

	bool operator!=(const Vector2& v) const 
	{ 
		return x != v.x || y != v.y; 
	}

	Vector2 operator+(const Vector2& b) const
	{
		return Vector2(x + b.x, y + b.y);
	}

	Vector2 operator-(const Vector2& b) const
	{
		return Vector2(x - b.x, y - b.y);
	}

	Vector2 operator*(float k) const
	{
		return Vector2(x * k, y * k);
	}

	Vector2 operator/(float k) const
	{
		float oneOverK = 1.0f / k;
		return Vector2(x * oneOverK, y * oneOverK);
	}

	Vector2& operator+=(const Vector2& b)
	{
		x += b.x;
		y += b.y;
		return *this;
	}

	Vector2& operator-=(const Vector2& b)
	{
		x -= b.x;
		y -= b.y;
		return *this;
	}

	Vector2& operator*=(float k)
	{
		x *= k;
		y *= k;
		return *this;
	}

	Vector2& operator/=(float k)
	{
		float oneOverK = 1.0f / k;
		x *= k;
		y *= k;
		return *this;
	}

	Vector2 operator-() const
	{
		return Vector2(-x, -y);
	}

	operator float*() { return &x; }
	operator const float*() const { return &x; }

	static float dot(const Vector2& a, const Vector2& b)
	{
		return a.x*b.x + a.y*b.y;
	}

	static float distance(const Vector2& a, const Vector2& b)
	{
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		return std::sqrt(dx*dx + dy*dy);
	}

	static Vector2 lerp(const Vector2& a, const Vector2& b, float t)
	{
		return(
			Vector2(
				a.x + t * (b.x - a.x),
				a.y + t * (b.y - a.y)
			)
		);
	}
};

inline Vector2 operator*(float k, const Vector2& v)
{
	return Vector2(k * v.x, k * v.y);
}

} // gdfx

#endif // __GDFX_MATH_VECTOR2_HPP__
