// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#include <gdfx/graphics/Font.hpp>
#include <gdfx/graphics/Texture.hpp>
#include <gdfx/content/ContentManager.hpp>

namespace gdfx {

Font::Font() :
	Content(),
	texture(nullptr),
	size(0),
	glyphs()
{
}

Font::~Font()
{
	unload();
}

void Font::createGlyphs()
{
	size = texture->getWidth() / GLYPHS_PER_ROW;

	for (int i = 0, x = 0, y = 0; i < MAX_GLYPHS; i++, x += size) {
		if (x >= texture->getWidth()) {
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

void Font::destroy()
{
	texture.reset();
	size = 0;
}

bool Font::load()
{
	destroy();

	texture = getContentMgr()->load<Texture>(getPath());
	if (texture == nullptr)
		return false;

	createGlyphs();
	return true;
}

void Font::unload()
{
	destroy();
}

} // gdfx


