//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_GRAPHICS_GRAPHICS_HPP__
#define __GDFX_GRAPHICS_GRAPHICS_HPP__

#include <gdfx/platform/SDL3.hpp>
#include <gdfx/platform/SDLException.hpp>

namespace gdfx {

class Graphics {
public:
    Graphics() :
        window(nullptr),
        renderer(nullptr),
        renderTarget(nullptr),
        width(0),
        height(0)
    {}

    ~Graphics()
    {
        SDL_DestroyTexture(renderTarget);
    }

    void create(const char *title, int w, int h)
    {
        if (!SDL_CreateWindowAndRenderer(title, w, h, SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED, &window, &renderer))
            throw SDLException();

        SDL_SetRenderLogicalPresentation(renderer, w, h, SDL_LOGICAL_PRESENTATION_LETTERBOX);

        renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
        SDL_SetTextureScaleMode(renderTarget, SDL_SCALEMODE_NEAREST);

        width = w;
        height = h;
    }

    void setColor(int red, int green, int blue, int alpha = 255)
    {
        SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
    }

    void clear()
    {
        SDL_RenderClear(renderer);
    }

    void drawText(const char *text, int x, int y)
    {
        SDL_RenderDebugText(renderer, x, y, text);
    }

    void begin()
    {
        setColor(0, 0, 0);
        clear();

        SDL_SetRenderTarget(renderer, renderTarget);
    }

    void end()
    {
        SDL_FRect dstRect{ 0, 0, (float)width, (float)height };
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderTexture(renderer, renderTarget, nullptr, &dstRect);
        SDL_RenderPresent(renderer);
    }

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *renderTarget;
    int width;
    int height;
};

} // gdfx

#endif // __GDFX_GRAPHICS_GRAPHICS_HPP__
