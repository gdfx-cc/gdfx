//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/graphics/Image.hpp>
#include <gdfx/graphics/Graphics.hpp>
#include <gdfx/platform/Storage.hpp>
#include <gdfx/platform/SDLException.hpp>
#include <gdfx/content/ContentManager.hpp>

namespace gdfx {

Image::Image() :
    Content(),
    texture(nullptr),
    width(0),
    height(0)
{
}

Image::~Image()
{
}

bool Image::load()
{
    int bytesPerPixel;
    uint8_t *data = Storage::loadImage(getPath().c_str(), width, height, bytesPerPixel, 4, false);
    if (!data)
        return false;

    SDL_Surface *surface = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA32, (void*)data, width * 4);
    if (!surface) {
        Storage::unloadImage(data);
        throw SDLException();
    }

    texture = SDL_CreateTextureFromSurface(getContentMgr()->getGraphics()->getRenderer(), surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        throw SDLException();
    }

    Storage::unloadImage(data);
    return true;
}

void Image::unload()
{
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    width = height = 0;
}

} // gdfx
