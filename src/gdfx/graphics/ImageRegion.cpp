//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/graphics/ImageRegion.hpp>

namespace gdfx {

ImageRegion::ImageRegion() :
    image(),
    x(0),
    y(0),
    width(0),
    height(0)
{
}

ImageRegion::ImageRegion(std::shared_ptr<Image> image, int x, int y, int width, int height) :
    image(image),
    x(x),
    y(y),
    width(width),
    height(height)
{
}

ImageRegion::~ImageRegion()
{
}

void ImageRegion::setRegion(std::shared_ptr<Image> image, int x, int y, int width, int height)
{
    this->image = image;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

} // gdfx
