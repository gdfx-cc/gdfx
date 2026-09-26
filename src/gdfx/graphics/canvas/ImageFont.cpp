// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#include <gdfx/graphics/canvas/ImageFont.hpp>
#include <gdfx/graphics/canvas/Image.hpp>
#include <gdfx/content/ContentManager.hpp>

namespace gdfx {

ImageFont::ImageFont() :
	Content(),
	image(nullptr),
	size(0),
	glyphs()
{
}

ImageFont::~ImageFont()
{
	unload();
}

void ImageFont::createGlyphs()
{
	size = image->getWidth() / GLYPHS_PER_ROW;

	for (int i = 0, x = 0, y = 0; i < MAX_GLYPHS; i++, x += size) {
		if (x >= image->getWidth()) {
			x = 0;
			y += size;
		}

		glyphs[i].character = 32 + i;
		glyphs[i].srcRect.x = x;
		glyphs[i].srcRect.y = y;
		glyphs[i].srcRect.w = size;
		glyphs[i].srcRect.h = size;
	}
}

void ImageFont::destroy()
{
	image.reset();
	size = 0;
}

bool ImageFont::load()
{
	destroy();

	image = getContentMgr()->load<Image>(getPath());
	if (image == nullptr)
		return false;

	createGlyphs();
	return true;
}

void ImageFont::unload()
{
	destroy();
}

} // gdfx


