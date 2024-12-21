//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_GRAPHICS_GRAPHICS_HPP__
#define __GDFX_GRAPHICS_GRAPHICS_HPP__

#include <gdfx/platform/SDL3.hpp>
#include <gdfx/graphics/Image.hpp>
#include <gdfx/graphics/ImageRegion.hpp>

namespace gdfx {

class Graphics {
public:
    Graphics();
    ~Graphics();

    void create(const char *title, int w, int h);
    void destroy();

    void begin();
    void end();

    void setColor(int red, int green, int blue, int alpha = 255);
    void clear();

    void drawText(const char *text, int x, int y);
    void drawPixel(int x, int y);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawRect(int x, int y, int w, int h);
    void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void drawCircle(int x, int y, int radius);
    void drawImage(Image& img, int x, int y);
    void drawImageRegion(ImageRegion& imageRegion, int x, int y);

    void drawFilledRect(int x, int y, int w, int h);
    void drawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void drawFilledCircle(int x, int y, int radius);

    SDL_Renderer *getRenderer() { return renderer; }

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *renderTarget;
    int width;
    int height;
};

} // gdfx

#endif // __GDFX_GRAPHICS_GRAPHICS_HPP__
