//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#ifndef GDFX_GRAPHICS_GRAPHICS_HPP
#define GDFX_GRAPHICS_GRAPHICS_HPP

#include <gdfx/platform/SDL3.hpp>
#include <gdfx/graphics/Color.hpp>
#include <gdfx/graphics/canvas/Canvas.hpp>
#include <gdfx/graphics/canvas/Palette.hpp>
#include <gdfx/graphics/Texture.hpp>
#include <gdfx/graphics/TextureRegion.hpp>
#include <gdfx/graphics/Sprite.hpp>
#include <gdfx/graphics/Font.hpp>

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
    void setColor(const Color& color);
    void clear();

    void drawPixel(int x, int y);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawRect(int x, int y, int w, int h);
    void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void drawCircle(int x, int y, int radius);
    
    void drawTexture(Texture& texture, int x, int y);
    void drawTexture(Texture& texture, int sx, int sy, int sw, int sh, int dx, int dy);
    void drawTextureRegion(TextureRegion& textureRegion, int x, int y);
    void drawSprite(Sprite& sprite, int x, int y);
    void drawCanvas(Canvas& canvas, float x = 0.0f, float y = 0.0f);
    void drawCanvas(Canvas& canvas, const Palette& palette, float x = 0.0f, float y = 0.0f);

    void drawFilledRect(int x, int y, int w, int h);
    void drawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void drawFilledCircle(int x, int y, int radius);

    void drawTextEx(Font *font, int x, int y, Font::HorizontalAlign hAlign, Font::VerticalAlign vAlign, int charsToDraw, const char* text);
    void drawText(Font *font, int x, int y, const char* fmt, ...);
    void drawTextCentered(Font *font, int x, int y, const char* fmt, ...);
    void drawTextRight(Font *font, int x, int y, const char* fmt, ...);

    SDL_Renderer *getRenderer() { return renderer; }

private:
    void updateCanvasTexture(Canvas& canvas, const Palette& palette);

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *renderTarget;
    SDL_Texture *canvasTexture;
    int canvasTextureWidth;
    int canvasTextureHeight;
    int width;
    int height;
};

} // gdfx

#endif // GDFX_GRAPHICS_GRAPHICS_HPP

