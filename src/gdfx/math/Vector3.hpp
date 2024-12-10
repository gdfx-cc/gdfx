//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_MATH_VECTOR3_HPP__
#define __GDFX_MATH_VECTOR3_HPP__

#include <cmath>

namespace gdfx {

/**
 * Vector3
 */
class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3() : x(0), y(0), z(0) {}
	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
	Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	Vector3& set(float nx, float ny, float nz) 
	{ 
		x = nx; y = ny; z = nz;
		return *this;
	}

	Vector3& set(const Vector3& v) { return set(v.x, v.y, v.z); }
	Vector3& zero() { x = 0; y = 0; z = 0; return *this; }
	float getLength() const { return std::sqrt(x*x + y*y + z*z); }

	Vector3& normalize()
	{
		float lengthSquared = x*x + y*y + z*z;
		if (lengthSquared > 0) {
			float inv = 1.0f /std::sqrt(lengthSquared);
			x *= inv;
			y *= inv;
			z *= inv;
		}
		return *this;
	}

	Vector3 getNormalized() const { Vector3 v(*this); return v.normalize(); }

	Vector3& operator=(const Vector3& v) { return set(v); }

	bool operator==(const Vector3& v) const 
	{ 
		return x == v.x && y == v.y && z == v.z; 
	}

	bool operator!=(const Vector3& v) const 
	{ 
		return x != v.x || y != v.y || z != v.z; 
	}

	Vector3 operator+(const Vector3& b) const
	{
		return Vector3(x + b.x, y + b.y, z + b.z);
	}

	Vector3 operator-(const Vector3& b) const
	{
		return Vector3(x - b.x, y - b.y, z - b.z);
	}

	Vector3 operator*(float k) const
	{
		return Vector3(x * k, y * k, z * k);
	}

	Vector3 operator/(float k) const
	{
		float oneOverK = 1.0f / k;
		return Vector3(x * oneOverK, y * oneOverK, z * oneOverK);
	}

	Vector3& operator+=(const Vector3& b)
	{
		x += b.x;
		y += b.y;
		z += b.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& b)
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		return *this;
	}

	Vector3& operator*=(float k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	Vector3& operator/=(float k)
	{
		float oneOverK = 1.0f / k;
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	operator float*() { return &x; }
	operator const float*() const { return &x; }

	static float dot(const Vector3& a, const Vector3& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	static void cross(const Vector3& a, const Vector3& b, Vector3& c)
	{
		c.set(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x 
			);
	}

	static Vector3 cross(const Vector3& a, const Vector3& b)
	{
		Vector3 c;
		cross(a, b, c);
		return c;
	}

	static float distance(const Vector3& a, const Vector3& b)
	{
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		float dz = a.z - b.z;
		return std::sqrt(dx*dx + dy*dy + dz*dz);
	}

	static Vector3 lerp(const Vector3& a, const Vector3& b, float t)
	{
		return(
			Vector3(
				a.x + t * (b.x - a.x),
				a.y + t * (b.y - a.y),
				a.z + t * (b.z - a.z)
			)
		);
	}

	Vector3& rotateX(float angle)
	{
		x = x;
		y = y * cos(angle) - z * sin(angle);
		z = y * sin(angle) + z * cos(angle);
		return *this;
	}

	Vector3& rotateY(float angle)
	{
		x = x * cos(angle) + z * sin(angle);
		y = y;
		z = -x * sin(angle) + z * cos(angle);
		return *this;
	}

	Vector3& rotateZ(float angle)
	{
		x = x * cos(angle) - y * sin(angle);
		y = x * sin(angle) + y * cos(angle);
		z = z;
		return *this;
	}

	Vector3 getRotatedX(float angle) const { Vector3 v(*this); return v.rotateX(angle); }
	Vector3 getRotatedY(float angle) const { Vector3 v(*this); return v.rotateY(angle); }
	Vector3 getRotatedZ(float angle) const { Vector3 v(*this); return v.rotateZ(angle); }
};

inline Vector3 operator*(float k, const Vector3& v)
{
	return Vector3(k * v.x, k * v.y, k * v.z);
}

} // gdfx

#endif // __GDFX_MATH_VECTOR3_HPP__
