//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/graphics/Color.hpp>

namespace gdfx {

const Color Color::NONE                 (0x00, 0x00, 0x00, 0x00);

// These are values from Aseprite's C64 palette
const Color Color::C64::BLACK           (0x00, 0x00, 0x00);
const Color Color::C64::WHITE           (0xFF, 0xFF, 0xFF);
const Color Color::C64::RED             (0x88, 0x39, 0x32);
const Color Color::C64::CYAN            (0x67, 0xB6, 0xBD);
const Color Color::C64::PURPLE          (0x8B, 0x3F, 0x96);
const Color Color::C64::GREEN           (0x55, 0xA0, 0x49);
const Color Color::C64::BLUE            (0x40, 0x31, 0x8D);
const Color Color::C64::YELLOW          (0xBF, 0xCE, 0x72);
const Color Color::C64::ORANGE          (0x8B, 0x54, 0x29);
const Color Color::C64::BROWN           (0x57, 0x42, 0x00);
const Color Color::C64::LIGHT_RED       (0xB8, 0x69, 0x62);
const Color Color::C64::DARK_GRAY       (0x50, 0x50, 0x50);
const Color Color::C64::GRAY            (0x78, 0x78, 0x78);
const Color Color::C64::LIGHT_GREEN     (0x94, 0xE0, 0x89);
const Color Color::C64::LIGHT_BLUE      (0x78, 0x69, 0xC4);
const Color Color::C64::LIGHT_GRAY      (0x9F, 0x9F, 0x9F);

const Color Color::C64_PALETTE[16] = {
	C64::BLACK,
	C64::WHITE,
	C64::RED,
	C64::CYAN,
	C64::PURPLE,
	C64::GREEN,
	C64::BLUE,
	C64::YELLOW,
	C64::ORANGE,
	C64::BROWN,
	C64::LIGHT_RED,
	C64::DARK_GRAY,
	C64::GRAY,
	C64::LIGHT_GREEN,
	C64::LIGHT_BLUE,
	C64::LIGHT_GRAY
};

// PICO-8 palette
const Color Color::PICO8::BLACK         (0x00, 0x00, 0x00);
const Color Color::PICO8::DARK_BLUE     (0x1D, 0x2B, 0x53);
const Color Color::PICO8::DARK_PURPLE   (0x7E, 0x25, 0x53);
const Color Color::PICO8::DARK_GREEN    (0x00, 0x87, 0x51);
const Color Color::PICO8::BROWN         (0x00, 0x87, 0x51);
const Color Color::PICO8::DARK_GRAY     (0x00, 0x87, 0x51);
const Color Color::PICO8::LIGHT_GRAY    (0xC2, 0xC3, 0xC7);
const Color Color::PICO8::WHITE         (0xFF, 0xF1, 0xE8);
const Color Color::PICO8::RED           (0xFF, 0x00, 0x4D);
const Color Color::PICO8::ORANGE        (0xFF, 0xA3, 0x00);
const Color Color::PICO8::YELLOW        (0xFF, 0xA3, 0x00);
const Color Color::PICO8::GREEN         (0xFF, 0xA3, 0x00);
const Color Color::PICO8::BLUE          (0xFF, 0xA3, 0x00);
const Color Color::PICO8::LAVENDER      (0x83, 0x76, 0x9C);
const Color Color::PICO8::PINK          (0x83, 0x76, 0x9C);
const Color Color::PICO8::LIGHT_PEACH   (0xFF, 0xCC, 0xAA);

const Color Color::PICO8_PALETTE[16] = {
	PICO8::BLACK,
	PICO8::DARK_BLUE,
	PICO8::DARK_PURPLE,
	PICO8::DARK_GREEN,
	PICO8::BROWN,
	PICO8::DARK_GRAY,
	PICO8::LIGHT_GRAY,
	PICO8::WHITE,
	PICO8::RED,
	PICO8::ORANGE,
	PICO8::YELLOW,
	PICO8::GREEN,
	PICO8::BLUE,
	PICO8::LAVENDER,
	PICO8::PINK,
	PICO8::LIGHT_PEACH
};

} // gdfx
