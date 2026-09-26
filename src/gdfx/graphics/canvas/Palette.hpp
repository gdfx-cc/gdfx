// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#ifndef GDFX_GRAPHICS_CANVAS_PALETTE_HPP
#define GDFX_GRAPHICS_CANVAS_PALETTE_HPP

#include <cstdint>
#include <vector>

#include <gdfx/graphics/Color.hpp>

namespace gdfx {

/**
 * Palette
 */
class Palette {
public:
	Palette();
	~Palette();

	int getSize() const { return (int)colors.size(); }
	Color getColor(int index) const;
	uint8_t getR(int index) const;
	uint8_t getG(int index) const;
	uint8_t getB(int index) const;
	uint8_t getA(int index) const;
	uint32_t getRGBA(int index) const;
	uint32_t getARGB(int index) const;
	uint32_t getABGR(int index) const;

	bool load(const char* file);
	bool save(const char* file);
	void resize(int size);
	void clear();
	void setColor(int index, const Color& c);
	void darkenColor(int index, int amount);
	void brightenColor(int index, int amount);
	void adjustColor(int index, int rAmount, int gAmount, int bAmount);

private:
	std::vector<Color> colors;
};

} // gdfx

#endif // GDFX_GRAPHICS_CANVAS_PALETTE_HPP
