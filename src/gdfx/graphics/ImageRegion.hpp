//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_GRAPHICS_IMAGEREGION_HPP__
#define __GDFX_GRAPHICS_IMAGEREGION_HPP__

#include <memory>

#include <gdfx/graphics/Image.hpp>

namespace gdfx {

class ImageRegion {
public:
    ImageRegion();
    ImageRegion(std::shared_ptr<Image> image, int x, int y, int width, int height);
    ~ImageRegion();

    void setRegion(std::shared_ptr<Image> image, int x, int y, int width, int height);

    Image *getImage() { return image.get(); }
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    std::shared_ptr<Image> image;
    int x;
    int y;
    int width;
    int height;
};

} // gdfx

#endif // __GDFX_GRAPHICS_IMAGEREGION_HPP__
