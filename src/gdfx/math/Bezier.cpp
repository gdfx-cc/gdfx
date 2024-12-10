//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <cassert>
#include <cmath>
#include <gdfx/math/Bezier.hpp>

namespace gdfx {

/**
* Calculates a series of points along a cubic Bezier curve, using the provided control points.
*
* @param controlPoints Array of 4 points used to control the Bezier curve, with p0 and p3 at the ends of the curve
* @param numDesiredPoints Number of points to calculate along the curve
* @param curvePoints The resulting vector of points calculated along the curve
*/
void Bezier::calcCurve(Vector2 controlPoints[4], int numDesiredPoints, std::vector<Vector2>& curvePoints)
{
	assert(numDesiredPoints >= 2);
	float wx[4], wy[4];
	
	// separate control points into distinct arrays for x and y weights
	for (int i = 0; i < 4; i++) {
		wx[i] = controlPoints[i].x;
		wy[i] = controlPoints[i].y;
	}
	
	for (int i = 0; i < numDesiredPoints; i++) {
		const float t = (float)i / (float)(numDesiredPoints - 1);
		const float x = cubicBezier(t, wx);
		const float y = cubicBezier(t, wy);

		Vector2 pt;
		pt.x = x;
		pt.y = y;
		curvePoints.push_back(pt);
	}
}

/**
* Calculates a series of points along a circular arc, using a Bezier curve for an approximation.
*
* @param center The center point of the circle
* @param radius How big the circle is from center to edge
* @param numDesiredPoints Number of points to calculate along the curve
* @param curvePoints The resulting vector of points calculated along the curve
*/
void Bezier::calcCircularArc(const Vector2& center, float radius, float angle, int numDesiredPoints, std::vector<Vector2>& curvePoints)
{
	const float k = (4.0f/3.0f) * std::tan(angle/4.0f);
	const float r = radius;
	const float x = center.x;
	const float y = center.y;
	const float c = std::cos(angle);
	const float s = std::sin(angle);

	Vector2 controlPoints[4] = {
		{ x + r, y + 0 },
		{ x + r, y + r * k },
		{ x + r * (c + k * s), y + r * (s - k * c) },
		{ x + r * c, y + r * s }
	};
	
	calcCurve(controlPoints, numDesiredPoints, curvePoints);
}

/**
* This is a cubic optimized version of a general purpose bezier function, with simplified
* math so as not to rely on more complicated math calculations.
*
* @param t A range between 0 and 1 (inclusive) along the curve
* @param w An array of 4 control point values (weights)
*
* @return Returns the point along the curve (1 dimensional)
*/
float Bezier::cubicBezier(float t, float w[4])
{
	const float t2 = t * t;
	const float t3 = t2 * t;
	const float mt = 1 - t;
	const float mt2 = mt * mt;
	const float mt3 = mt2 * mt;

	return w[0]*mt3 + 3*w[1]*mt2*t + 3*w[2]*mt*t2 + w[3]*t3;
}

} // gdfx
