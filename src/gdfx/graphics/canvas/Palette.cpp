// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#include <gdfx/graphics/canvas/Palette.hpp>

namespace gdfx {

Palette::Palette() :
	colors{}
{
}

Palette::~Palette()
{
}

Color Palette::getColor(int index) const
{
	assert(index >= 0 && index < colors.size());
	return colors[index];
}

uint8_t Palette::getR(int index) const
{
	return getColor(index).r;
}

uint8_t Palette::getG(int index) const
{
	return getColor(index).g;
}

uint8_t Palette::getB(int index) const
{
	return getColor(index).b;
}

uint8_t Palette::getA(int index) const
{
	return getColor(index).a;
}

uint32_t Palette::getRGBA(int index) const
{
	return getColor(index).getRGBA();
}

uint32_t Palette::getARGB(int index) const
{
	return getColor(index).getARGB();
}

uint32_t Palette::getABGR(int index) const
{
	return getColor(index).getABGR();
}

/*
JASC-PAL
0100
64
124 124 124
0 0 252
0 0 188
68 40 188
*/
bool Palette::load(const char* file)
{
	const int BUFFER_SIZE = 16;
	char buffer[BUFFER_SIZE];

	FILE* fp = fopen(file, "rt");
	if (!fp)
		return false;

	if (!fgets(buffer, BUFFER_SIZE, fp) || strcmp(buffer, "JASC-PAL\n") != 0 ||
		!fgets(buffer, BUFFER_SIZE, fp) || strcmp(buffer, "0100\n") != 0 ||
		!fgets(buffer, BUFFER_SIZE, fp)) {
		fclose(fp);
		return false;
	}

	int r, g, b;
	int size = atoi(buffer);
	for (int i = 0; i < size; i++) {
		if (fscanf(fp, "%d %d %d", &r, &g, &b) != 3)
			break;
		colors.push_back(Color{ (uint8_t)r, (uint8_t)g, (uint8_t)b });
	}

	fclose(fp);
	if (getSize() != size)
		return false;

	return true;
}

/*
JASC-PAL
0100
64
124 124 124
0 0 252
0 0 188
68 40 188
*/
bool Palette::save(const char* file)
{
	FILE* fp = fopen(file, "wt");
	if (!fp)
		return false;

	fprintf(fp, "JASC-PAL\n");
	fprintf(fp, "0100\n");
	fprintf(fp, "%d\n", getSize());

	for (Color& c : colors)
		fprintf(fp, "%d %d %d\n", c.r, c.g, c.b);

	fclose(fp);
	return true;
}

void Palette::resize(int size)
{
	assert(size > 0);

	// shrink
	while (colors.size() > size)
		colors.pop_back();

	// grow
	while (size > colors.size())
		colors.push_back(Color{ 0, 0, 0, 255 });
}

void Palette::clear()
{
	for (Color& c : colors)
		c.clear();
}

void Palette::setColor(int index, const Color& c)
{
	assert(index >= 0 && index < colors.size());
	colors[index] = c;
}

void Palette::darkenColor(int index, int amount)
{
	adjustColor(index, -amount, -amount, -amount);
}

void Palette::brightenColor(int index, int amount)
{
	adjustColor(index, amount, amount, amount);
}

void Palette::adjustColor(int index, int rAmount, int gAmount, int bAmount)
{
	assert(index >= 0 && index < colors.size());

	int r = std::min(std::max(colors[index].r + rAmount, 0), 255);
	int g = std::min(std::max(colors[index].g + gAmount, 0), 255);
	int b = std::min(std::max(colors[index].b + bAmount, 0), 255);

	colors[index].r = (uint8_t)r;
	colors[index].g = (uint8_t)g;
	colors[index].b = (uint8_t)b;
}

} // gdfx


