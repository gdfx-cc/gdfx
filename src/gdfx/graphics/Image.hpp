//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_GRAPHICS_IMAGE_HPP__
#define __GDFX_GRAPHICS_IMAGE_HPP__

#include <gdfx/platform/SDL3.hpp>
#include <gdfx/content/Content.hpp>

namespace gdfx {

class Image : public Content {
protected:
    // protected ctor, use Image::create() instead
    Image();

public:
    virtual ~Image();

    // for ContentManager
    static Image *create() { return new Image(); }

    virtual bool load() override;
    virtual void unload() override;

    SDL_Texture *getTexture() { return texture; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    SDL_Texture *texture;
    int width;
    int height;
};

} // gdfx

#endif // __GDFX_GRAPHICS_IMAGE_HPP__
