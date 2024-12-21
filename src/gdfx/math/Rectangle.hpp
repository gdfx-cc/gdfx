//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_MATH_RECTANGLE_HPP__
#define __GDFX_MATH_RECTANGLE_HPP__

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
		if (x + w < r.x)
			return false;
		if (x > r.x + r.w)
			return false;

		if (y + h < r.y)
			return false;
		if (y > r.y + r.h)
			return false;

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

#endif // __GDFX_MATH_RECTANGLE_HPP__
