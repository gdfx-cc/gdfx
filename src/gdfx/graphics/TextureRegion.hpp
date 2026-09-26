//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#ifndef GDFX_GRAPHICS_TEXTUREREGION_HPP
#define GDFX_GRAPHICS_TEXTUREREGION_HPP

#include <memory>

#include <gdfx/graphics/Texture.hpp>

namespace gdfx {

class TextureRegion {
public:
    TextureRegion();
    TextureRegion(std::shared_ptr<Texture> texture, int x, int y, int width, int height);
    ~TextureRegion();

    void setRegion(std::shared_ptr<Texture> texture, int x, int y, int width, int height);

    Texture *getTexture() { return texture.get(); }
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    std::shared_ptr<Texture> texture;
    int x;
    int y;
    int width;
    int height;
};

} // gdfx

#endif // GDFX_GRAPHICS_TEXTUREREGION_HPP
