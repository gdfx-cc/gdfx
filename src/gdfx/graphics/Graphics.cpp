//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/platform/SDLException.hpp>
#include <gdfx/graphics/Graphics.hpp>

namespace gdfx {

Graphics::Graphics() :
    window(nullptr),
    renderer(nullptr),
    renderTarget(nullptr),
    width(0),
    height(0)
{
}

Graphics::~Graphics()
{
    if (renderTarget) {
        SDL_DestroyTexture(renderTarget);
        renderTarget = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

void Graphics::create(const char *title, int w, int h)
{
    if (!SDL_CreateWindowAndRenderer(title, w, h, SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED, &window, &renderer))
        throw SDLException();

    SDL_SetRenderLogicalPresentation(renderer, w, h, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    //SDL_SetRenderLogicalPresentation(renderer, w, h, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

    renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureScaleMode(renderTarget, SDL_SCALEMODE_NEAREST);

    width = w;
    height = h;
}

void Graphics::begin()
{
    setColor(0, 0, 0);
    clear();

    SDL_SetRenderTarget(renderer, renderTarget);
}

void Graphics::end()
{
    SDL_FRect dstRect{ 0, 0, (float)width, (float)height };
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderTexture(renderer, renderTarget, nullptr, &dstRect);
    SDL_RenderPresent(renderer);
}

void Graphics::setColor(int red, int green, int blue, int alpha)
{
    SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
}

void Graphics::clear()
{
    SDL_RenderClear(renderer);
}

void Graphics::drawText(const char *text, int x, int y)
{
    SDL_RenderDebugText(renderer, x, y, text);
}

void Graphics::drawPixel(int x, int y)
{
    SDL_RenderPoint(renderer, (float)x, (float)y);
}

void Graphics::drawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderLine(renderer, (float)x1, (float)y1, (float)x2, (float)y2);
}

void Graphics::drawRect(int x, int y, int w, int h)
{
    SDL_FRect rect{ (float)x, (float)y, (float)w, (float)h };
    SDL_RenderRect(renderer, &rect);
}

void Graphics::fillRect(int x, int y, int w, int h)
{
    SDL_FRect rect{ (float)x, (float)y, (float)w, (float)h };
    SDL_RenderFillRect(renderer, &rect);
}

} // gdfx
