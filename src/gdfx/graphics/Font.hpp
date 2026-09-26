// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#ifndef GDFX_GRAPHICS_FONT_HPP
#define GDFX_GRAPHICS_FONT_HPP

#include <memory>
#include <gdfx/content/Content.hpp>
#include <gdfx/math/Rectangle.hpp>

namespace gdfx {

class Texture;

/**
* Font
*/
class Font : public Content {
protected:
	// protected ctor, fonts should only be loaded via content.load<Font>("path/to/font.png");
	Font();

public:
	static const int GLYPHS_PER_ROW = 16;
	static const int MAX_GLYPHS = 256;

	struct Glyph {
		int character;
		Rectangle srcRect;

		Glyph() : character{}, srcRect{} {}
	};

	enum class HorizontalAlign {
		LEFT,
		CENTER,
		RIGHT
	};

	enum class VerticalAlign {
		TOP,
		CENTER,
		BOTTOM
	};

	virtual ~Font();

	// for ContentManager
	static Font *create() { return new Font(); }

	void createGlyphs();
	void destroy();

	virtual bool load() override;
	virtual void unload() override;

	Texture *getTexture() { return texture.get(); }
	int getSize() const { return size; }
	const Glyph& getGlyph(int index) const { return glyphs[index]; }

private:
	std::shared_ptr<Texture> texture;
	int size;
	Glyph glyphs[MAX_GLYPHS];
};

} // gdfx

#endif // GDFX_GRAPHICS_FONT_HPP
