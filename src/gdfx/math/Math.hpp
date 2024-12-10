//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_MATH_MATH_HPP__
#define __GDFX_MATH_MATH_HPP__

namespace gdfx {

/**
* Math
*/
class Math {
public:
	Math() = delete;
	~Math() = delete;

	static const double PI;
	static const double PI_2;
	static const double PI_4;

	static float degreesToRadians(float degrees)
	{
		return (float)(degrees * PI / 180.0f);
	}

	static float lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	static float easeInQuad(float t)
	{
		return t * t;
	}

	static float easeInCubic(float t)
	{
		return t * t * t;
	}

	static float easeInQuart(float t)
	{
		return t * t * t * t;
	}

	static float easeInQuint(float t)
	{
		return t * t * t * t * t;
	}
};

} // gdfx

#endif // __GDFX_MATH_MATH_HPP__

