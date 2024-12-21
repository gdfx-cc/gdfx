//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_GRAPHICS_COLOR_HPP__
#define __GDFX_GRAPHICS_COLOR_HPP__

#include <cstdint>
#include <cstring>
#include <cstdlib>

namespace gdfx {

/**
 * Color
 */
class Color {
public:
	static const Color NONE;
	static const Color BLACK;
	static const Color WHITE;

	struct C64 {
		static const Color BLACK;
		static const Color WHITE;
		static const Color RED;
		static const Color CYAN;
		static const Color PURPLE;
		static const Color GREEN;
		static const Color BLUE;
		static const Color YELLOW;
		static const Color ORANGE;
		static const Color BROWN;
		static const Color LIGHT_RED;
		static const Color DARK_GRAY;
		static const Color GRAY;
		static const Color LIGHT_GREEN;
		static const Color LIGHT_BLUE;
		static const Color LIGHT_GRAY;
	};
	static const Color C64_PALETTE[16];

	struct PICO8 {
		static const Color BLACK;
		static const Color DARK_BLUE;
		static const Color DARK_PURPLE;
		static const Color DARK_GREEN;
		static const Color BROWN;
		static const Color DARK_GRAY;
		static const Color LIGHT_GRAY;
		static const Color WHITE;
		static const Color RED;
		static const Color ORANGE;
		static const Color YELLOW;
		static const Color GREEN;
		static const Color BLUE;
		static const Color LAVENDER;
		static const Color PINK;
		static const Color LIGHT_PEACH;
	};
	static const Color PICO8_PALETTE[16];

public:
	int r;
	int g;
	int b;
	int a;

	Color() : r(0), g(0), b(0), a(0) {}
	Color(const Color& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}

	Color(int red, int green, int blue, int alpha = 255)
		: r(red), g(green), b(blue), a(alpha) {}

	Color& set(int red, int green, int blue, int alpha = 255)
	{ 
		r = red; g = green; b = blue; a = alpha;
		return *this;
	}

	/* #rrggbb or rrggbb */
	Color& set(const char* hexCode)
	{
		if (hexCode != nullptr && std::strlen(hexCode) >= 6) {
			if (*hexCode == '#')
				hexCode++;
			unsigned long code = std::strtoul(hexCode, NULL, 16);
			set((int)((code & 0xff0000) >> 16), (int)((code & 0xff00) >> 8), (int)(code & 0xff));
		}
		return *this;
	}

	Color& set(const Color& c) { return set(c.r, c.g, c.b, c.a); }
	Color& clear() { r = g = b = 0; a = 255; return *this; }

	int getRed() const { return r; }
	int getGreen() const { return g; }
	int getBlue() const { return b; }
	int getAlpha() const { return a; }

	uint32_t getRGBA() const
	{
		return (uint32_t)((r << 24) | (g << 16) | (b << 8) | (a));
	}

	uint32_t getARGB() const
	{
		return (uint32_t)((a << 24) | (r << 16) | (g << 8) | (b));
	}

	uint32_t getABGR() const
	{
		return (uint32_t)((a << 24) | (b << 16) | (g << 8) | (r));
	}

	Color& operator=(const Color& c) { return set(c); }

	bool operator==(const Color& c) const 
	{ 
		return r == c.r && g == c.g && b == c.b && a == c.a;
	}

	bool operator!=(const Color& c) const 
	{ 
		return r != c.r || g != c.g || b != c.b || a != c.a;
	}

	operator int*() { return &r; }
	operator const int*() const { return &r; }
};

} // gdfx

#endif // __GDFX_GRAPHICS_COLOR_HPP__
