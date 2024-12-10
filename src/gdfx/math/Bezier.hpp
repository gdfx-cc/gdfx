//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_MATH_BEZIER_HPP__
#define __GDFX_MATH_BEZIER_HPP__

#include <vector>
#include <gdfx/math/Vector2.hpp>

namespace gdfx {

/**
 * Bezier
 * https://pomax.github.io/bezierinfo
 */
class Bezier {
public:
	static void calcCurve(Vector2 controlPoints[4], int numDesiredPoints, std::vector<Vector2>& curvePoints);
	static void calcCircularArc(const Vector2& center, float radius, float angle, int numDesiredPoints, std::vector<Vector2>& curvePoints);
	static float cubicBezier(float t, float w[4]);
};

} // gdfx

#endif // __GDFX_MATH_BEZIER_HPP__
