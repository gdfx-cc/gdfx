//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/graphics/TextureRegion.hpp>

namespace gdfx {

TextureRegion::TextureRegion() :
    texture(),
    x(0),
    y(0),
    width(0),
    height(0)
{
}

TextureRegion::TextureRegion(std::shared_ptr<Texture> texture, int x, int y, int width, int height) :
    texture(texture),
    x(x),
    y(y),
    width(width),
    height(height)
{
}

TextureRegion::~TextureRegion()
{
}

void TextureRegion::setRegion(std::shared_ptr<Texture> texture, int x, int y, int width, int height)
{
    this->texture = texture;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

} // gdfx
