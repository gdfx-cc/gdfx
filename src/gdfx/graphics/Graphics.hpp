//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_GRAPHICS_GRAPHICS_HPP__
#define __GDFX_GRAPHICS_GRAPHICS_HPP__

#include <gdfx/platform/SDL3.hpp>

namespace gdfx {

class Graphics {
public:
    Graphics();
    ~Graphics();

    void create(const char *title, int w, int h);
    void begin();
    void end();

    void setColor(int red, int green, int blue, int alpha = 255);
    void clear();

    void drawText(const char *text, int x, int y);
    void drawPixel(int x, int y);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawRect(int x, int y, int w, int h);
    
    void fillRect(int x, int y, int w, int h);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *renderTarget;
    int width;
    int height;
};

} // gdfx

#endif // __GDFX_GRAPHICS_GRAPHICS_HPP__
