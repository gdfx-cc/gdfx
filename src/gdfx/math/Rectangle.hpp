//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#ifndef GDFX_MATH_RECTANGLE_HPP
#define GDFX_MATH_RECTANGLE_HPP

#include <algorithm>

namespace gdfx {

/**
 * Rectangle
 */
class Rectangle {
public:
	int x;
	int y;
	int w;
	int h;

	Rectangle() : x(0), y(0), w(0), h(0) {}
	Rectangle(const Rectangle& r) : x(r.x), y(r.y), w(r.w), h(r.h) {}
	Rectangle(int nx, int ny, int nw, int nh) : x(nx), y(ny), w(nw), h(nh) {}

	Rectangle& set(int nx, int ny, int nw, int nh)
	{
		x = nx; y = ny; w = nw; h = nh;
		return *this;
	}

	Rectangle& set(const Rectangle& r)
	{
		return set(r.x, r.y, r.w, r.h);
	}

	Rectangle& makeZero()
	{
		x = y = w = h = 0;
		return *this;
	}

	bool intersects(const Rectangle& r) const
	{
		return (r.x < x + w &&
				x < r.x + r.w &&
				r.y < y + h &&
				y < r.y + r.h);
	}

	static bool intersect(const Rectangle& r1, const Rectangle& r2, Rectangle& result)
	{
		if (!r1.intersects(r2)) {
			result.makeZero();
			return false;
		}

		const int right = std::min(r1.x + r1.w, r2.x + r2.w);
		const int left = std::max(r1.x, r2.x);
		const int top = std::max(r1.y, r2.y);
		const int bottom = std::min(r1.y + r1.h, r2.y + r2.h);

		result.set(left, top, right - left, bottom - top);
		return true;
	}

	Rectangle& operator=(const Rectangle& r)
	{
		return set(r);
	}

	bool operator==(const Rectangle& r) const
	{
		return x == r.x && y == r.y && w == r.w && h == r.h;
	}

	bool operator!=(const Rectangle& r) const
	{
		return x != r.x || y != r.y || w != r.w || h != r.h;
	}
};

} // gdfx

#endif // GDFX_MATH_RECTANGLE_HPP
