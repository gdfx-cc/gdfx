// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#ifndef GDFX_GRAPHICS_CANVAS_IMAGEFONT_HPP
#define GDFX_GRAPHICS_CANVAS_IMAGEFONT_HPP

#include <memory>
#include <gdfx/content/Content.hpp>
#include <gdfx/math/Rectangle.hpp>

namespace gdfx {

class Image;

/**
* ImageFont
*/
class ImageFont : public Content {
protected:
	// protected ctor, image fonts should only be loaded via content.load<ImageFont>("path/to/font.pcx");
	ImageFont();

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

	virtual ~ImageFont();

	// for ContentManager
	static ImageFont *create() { return new ImageFont(); }

	void createGlyphs();
	void destroy();

	virtual bool load() override;
	virtual void unload() override;

	Image *getImage() { return image.get(); }
	int getSize() const { return size; }
	const Glyph& getGlyph(int index) const { return glyphs[index]; }

private:
	std::shared_ptr<Image> image;
	int size;
	Glyph glyphs[MAX_GLYPHS];
};

} // gdfx

#endif // GDFX_GRAPHICS_CANVAS_IMAGEFONT_HPP
