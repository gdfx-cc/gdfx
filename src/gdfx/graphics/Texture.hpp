//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#ifndef GDFX_GRAPHICS_TEXTURE_HPP
#define GDFX_GRAPHICS_TEXTURE_HPP

#include <gdfx/platform/SDL3.hpp>
#include <gdfx/content/Content.hpp>

namespace gdfx {

class Texture : public Content {
protected:
    // protected ctor, use Texture::create() instead
    Texture();

public:
    virtual ~Texture();

    // for ContentManager
    static Texture *create() { return new Texture(); }

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

#endif // GDFX_GRAPHICS_TEXTURE_HPP
