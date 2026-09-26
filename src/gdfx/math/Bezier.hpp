//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#ifndef GDFX_MATH_BEZIER_HPP
#define GDFX_MATH_BEZIER_HPP

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

#endif // GDFX_MATH_BEZIER_HPP
