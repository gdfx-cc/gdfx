// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#ifndef GDFX_GRAPHICS_CANVAS_CANVAS_HPP
#define GDFX_GRAPHICS_CANVAS_CANVAS_HPP

#include <cstdint>
#include <fstream>
#include <vector>
#include <gdfx/math/Rectangle.hpp>
#include <gdfx/math/Vector2.hpp>
#include <gdfx/math/LineClipper.hpp>
#include <gdfx/graphics/canvas/Palette.hpp>
#include <gdfx/graphics/canvas/ImageFont.hpp>

namespace gdfx {

class Image;

/**
 * Canvas - A software rendering surface with clipping and drawing primitives.
 */
class Canvas {
public:
    static const int MAX_NUM_POLY2D_VERTICES = 100;

    struct Triangle2D {
        Vector2 vertices[3];
    };

    struct Polygon2D {
        Vector2 vertices[MAX_NUM_POLY2D_VERTICES];
        int numVertices;
    };

public:
    Canvas();
    ~Canvas();

    void create(int w, int h);
    void destroy();
    uint8_t *getBuffer();
    Palette& getPalette();

    int getWidth() const;
    int getHeight() const;

    void setClipRect(int x, int y, int w, int h);
    const Rectangle& getClipRect() const;

    void clear();
    void clearToColor(int color);

    void drawPixel(int x, int y, int color);
    void drawLine(int x1, int y1, int x2, int y2, int color);
    void drawHLine(int x1, int y1, int x2, int color);
    void drawVLine(int x1, int y1, int y2, int color);
    void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color);
    void drawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color);
    void drawFilledFBTriangle(int x1, int y1, int x2, int y23, int x3, int, int color);
    void drawRectangle(int x, int y, int w, int h, int color);
    void drawFilledRectangle(int x, int y, int w, int h, int color);
    void drawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int color);
    void drawFilledQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int color);
    void drawCircle(int xOrigin, int yOrigin, int radius, int color);
    void drawCirclePoints(int xOrigin, int yOrigin, int x, int y, int color);
    void drawCanvas(Canvas& canvas, int sx, int sy, int sw, int sh, int dx, int dy);
    void drawMaskedCanvas(Canvas& canvas, int sx, int sy, int sw, int sh, int dx, int dy);
    void drawMaskedColorModCanvas(Canvas& canvas, int sx, int sy, int sw, int sh, int dx, int dy, int color);
    void drawImage(Image *image, int sx, int sy, int sw, int sh, int dx, int dy);
    void drawMaskedImage(Image *image, int sx, int sy, int sw, int sh, int dx, int dy);
    void drawMaskedColorModImage(Image *image, int sx, int sy, int sw, int sh, int dx, int dy, int color);
    void drawTextEx(ImageFont *font, int x, int y, int color, ImageFont::HorizontalAlign hAlign, ImageFont::VerticalAlign vAlign, int charsToDraw, const char* text);
    void drawText(ImageFont *font, int x, int y, int color, const char* fmt, ...);
    void drawTextCentered(ImageFont *font, int x, int y, int color, const char* fmt, ...);
    void drawTextRight(ImageFont *font, int x, int y, int color, const char* fmt, ...);
    void drawBezier(Vector2 controlPoints[4], int numDesiredPoints, int color);
    void drawCircleArc(const Vector2& center, int radius, float angle, int numDesiredPoints, int color);
    void drawFilledCircleArc(const Vector2& center, int radius, float angle, int numDesiredPoints, int color, bool includeCenterPoint = false);

    void drawPixelNoClip(int x, int y, int color);
    void drawLineNoClip(int x1, int y1, int x2, int y2, int color);
    void drawHLineNoClip(int x1, int y1, int x2, int color);
    void drawVLineNoClip(int x1, int y1, int y2, int color);
    void drawBresenhamLineOctant0NoClip(int x0, int y0, int dx, int dy, int direction, int color);
    void drawBresenhamLineOctant1NoClip(int x0, int y0, int dx, int dy, int direction, int color);
    void drawBresenhamLineNoClip(int x0, int y0, int x1, int y1, int color);

protected:
    void triangulate2D(const Polygon2D& poly, std::vector<Triangle2D>& triangles);

private:
    uint8_t *buffer;
    int width;
    int height;
    int clipX;
    int clipY;
    int clipX2;
    int clipY2;
    Rectangle clipRect;
    LineClipper lineClipper;
    Palette palette;
};

} // gdfx

#endif // GDFX_GRAPHICS_CANVAS_CANVAS_HPP

