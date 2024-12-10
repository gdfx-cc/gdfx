//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_MATH_VECTOR4_HPP__
#define __GDFX_MATH_VECTOR4_HPP__

#include <cmath>
#include <gdfx/math/Vector3.hpp>

namespace gdfx {

/**
 * Vector4
 */
class Vector4 {
public:
	float x;
	float y;
	float z;
	float w;

	Vector4() : x(0), y(0), z(0), w(0) {}
	Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	Vector4(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}

	Vector4& set(float nx, float ny, float nz, float nw) 
	{ 
		x = nx; y = ny; z = nz; w = nw;
		return *this;
	}

	Vector4& set(const Vector4& v) { return set(v.x, v.y, v.z, v.w); }
	Vector4& zero() { x = 0; y = 0; z = 0; w = 0; return *this; }
	float getLength() const { return std::sqrt(x*x + y*y + z*z + w*w); }

	Vector4& normalize()
	{
		float lengthSquared = x*x + y*y + z*z + w*w;
		if (lengthSquared > 0) {
			float inv = 1.0f / std::sqrt(lengthSquared);
			x *= inv;
			y *= inv;
			z *= inv;
			w *= inv;
		}
		return *this;
	}

	Vector4 getNormalized() const { Vector4 v(*this); return v.normalize(); }

	Vector4& operator=(const Vector4& v) { return set(v); }

	bool operator==(const Vector4& v) const 
	{ 
		return x == v.x && y == v.y && z == v.z && w == v.w; 
	}

	bool operator!=(const Vector4& v) const 
	{ 
		return x != v.x || y != v.y || z != v.z || w != v.w; 
	}

	Vector4 operator+(const Vector4& b) const
	{
		return Vector4(x + b.x, y + b.y, z + b.z, w + b.w);
	}

	Vector4 operator-(const Vector4& b) const
	{
		return Vector4(x - b.x, y - b.y, z - b.z, w - b.w);
	}

	Vector4 operator*(float k) const
	{
		return Vector4(x * k, y * k, z * k, w * k);
	}

	Vector4 operator/(float k) const
	{
		float oneOverK = 1.0f / k;
		return Vector4(x * oneOverK, y * oneOverK, z * oneOverK, w * oneOverK);
	}

	Vector4& operator+=(const Vector4& b)
	{
		x += b.x;
		y += b.y;
		z += b.z;
		w += b.w;
		return *this;
	}

	Vector4& operator-=(const Vector4& b)
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		w -= b.w;
		return *this;
	}

	Vector4& operator*=(float k)
	{
		x *= k;
		y *= k;
		z *= k;
		w *= k;
		return *this;
	}

	Vector4& operator/=(float k)
	{
		float oneOverK = 1 / k;
		x *= k;
		y *= k;
		z *= k;
		w *= k;
		return *this;
	}

	Vector4 operator-() const
	{
		return Vector4(-x, -y, -z, -w);
	}

	operator float*() { return &x; }
	operator const float*() const { return &x; }

	static Vector3 toVector3(const Vector4& v)
	{
		return Vector3(v.x, v.y, v.z);
	}

	static Vector4 fromVector3(const Vector3& v)
	{
		return Vector4(v.x, v.y, v.z, 1.0f);
	}

	static float dot(const Vector4& a, const Vector4& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	}

	static float distance(const Vector4& a, const Vector4& b)
	{
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		float dz = a.z - b.z;
		float dw = a.w - b.w;
		return std::sqrt(dx*dx + dy*dy + dz*dz + dw*dw);
	}

	static Vector4 lerp(const Vector4& a, const Vector4& b, float t)
	{
		return(
			Vector4(
				a.x + t * (b.x - a.x),
				a.y + t * (b.y - a.y),
				a.z + t * (b.z - a.z),
				a.w + t * (b.w - a.w)
			)
		);
	}
};

inline Vector4 operator*(float k, const Vector4& v)
{
	return Vector4(k * v.x, k * v.y, k * v.z, k * v.w);
}

} // gdfx

#endif // __GDFX_MATH_VECTOR4_HPP__
