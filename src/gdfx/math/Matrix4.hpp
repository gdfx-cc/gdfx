//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_MATH_MATRIX4_HPP__
#define __GDFX_MATH_MATRIX4_HPP__

#include <cmath>
#include <gdfx/math/Math.hpp>
#include <gdfx/math/Vector4.hpp>

namespace gdfx {

/**
 * Matrix4
 */
class Matrix4 {
public:
	float m[4][4];

	Matrix4() : m{} {}
	Matrix4(const Matrix4& mat4)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m[i][j] = mat4.m[i][j];
			}
		}
	}

	Matrix4& zero()
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m[i][j] = 0.0f;
			}
		}
		return *this;
	}

	Matrix4& set(
		float translateX, float translateY, float translateZ,
		float rotateXDegrees, float rotateYDegrees, float rotateZDegrees,
		float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f)
	{
		Matrix4 scaleMatrix, rotateZMatrix, rotateYMatrix, rotateXMatrix, translateMatrix;

		scaleMatrix.makeScale(scaleX, scaleY, scaleZ);
		rotateZMatrix.makeRotationZ(Math::degreesToRadians(rotateZDegrees));
		rotateYMatrix.makeRotationY(Math::degreesToRadians(rotateYDegrees));
		rotateXMatrix.makeRotationX(Math::degreesToRadians(rotateXDegrees));
		translateMatrix.makeTranslation(translateX, translateY, translateZ);

		*this = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
		return *this;
	}

	Matrix4& makeIdentity()
	{
		zero();
		for (int i = 0; i < 4; i++)
			m[i][i] = 1.0f;
		return *this;
	}

	Matrix4& makeScale(float sx, float sy, float sz)
	{
		makeIdentity();
		m[0][0] = sx;
		m[1][1] = sy;
		m[2][2] = sz;
		return *this;
	}

	Matrix4& makeTranslation(float tx, float ty, float tz)
	{
		makeIdentity();
		m[0][3] = tx;
		m[1][3] = ty;
		m[2][3] = tz;
		return *this;
	}

	Matrix4& makeRotationX(float angle)
	{
		float c = cos(angle);
		float s = sin(angle);

		makeIdentity();
		m[1][1] = c;
		m[1][2] = -s;
		m[2][1] = s;
		m[2][2] = c;
		return *this;
	}

	Matrix4& makeRotationY(float angle)
	{
		float c = cos(angle);
		float s = sin(angle);

		makeIdentity();
		m[0][0] = c;
		m[0][2] = s;
		m[2][0] = -s;
		m[2][2] = c;
		return *this;
	}

	Matrix4& makeRotationZ(float angle)
	{
		float c = cos(angle);
		float s = sin(angle);

		makeIdentity();
		m[0][0] = c;
		m[0][1] = -s;
		m[1][0] = s;
		m[1][1] = c;
		return *this;
	}

	Matrix4& makePerspective(float fov, float aspect, float znear, float zfar)
	{
		zero();
		m[0][0] = aspect * (1 / tan(fov / 2));
		m[1][1] = 1 / tan(fov / 2);
		m[2][2] = zfar / (zfar - znear);
		m[2][3] = (-zfar * znear) / (zfar - znear);
		m[3][2] = 1.0;
		return *this;
	}

	// https://en.wikipedia.org/wiki/Orthographic_projection
	Matrix4& makeOrtho(float left, float right, float bottom, float top, float znear, float zfar)
	{
		zero();
		m[0][0] =  2 / (right - left);
		m[1][1] =  2 / (top - bottom);
		m[2][2] = -2 / (zfar - znear);
		m[3][0] = -(right + left) / (right - left);
		m[3][1] = -(top + bottom) / (top - bottom);
		m[3][2] = -(zfar + znear) / (zfar - znear);
		m[3][3] = 1.0f;
		return *this;
	}

	Matrix4& lookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
	{
		zero();

		Vector3 z = target - eye;
		z.normalize();

		Vector3 x = Vector3::cross(up, z);
		x.normalize();

		Vector3 y = Vector3::cross(z, x);

		// | x.x   x.y   x.z   -dot(x,eye) |
		// | y.x   y.y   y.z   -dot(y,eye) |
		// | z.x   z.y   z.z   -dot(z,eye) |
		// |   0     0     0             1 |

		m[0][0] = x.x; m[0][1] = x.y; m[0][2] = x.z; m[0][3] = -Vector3::dot(x, eye);
		m[1][0] = y.x; m[1][1] = y.y; m[1][2] = y.z; m[1][3] = -Vector3::dot(y, eye);
		m[2][0] = z.x; m[2][1] = z.y; m[2][2] = z.z; m[2][3] = -Vector3::dot(z, eye);
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

		return *this;
	}

	Vector4 multiplyAndProject(const Vector4& v) const
	{
		Vector4 result = *this * v;

		if (result.w != 0.0f) {
			result.x /= result.w;
			result.y /= result.w;
			result.z /= result.w;
		}
		return result;
	}

	Vector4 operator*(const Vector4& v) const
	{
		Vector4 result;

		result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
		result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
		result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
		result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;

		return result;
	}

	Matrix4 operator*(const Matrix4& b) const
	{
		Matrix4 result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.m[i][j] = m[i][0] * b.m[0][j] + m[i][1] * b.m[1][j] + m[i][2] * b.m[2][j] + m[i][3] * b.m[3][j];
			}
		}
		return result;
	}

	operator float*() { return &m[0][0]; }
	operator const float*() const { return &m[0][0]; }
};

} // gdfx

#endif // __GDFX_MATH_MATRIX4_HPP__
