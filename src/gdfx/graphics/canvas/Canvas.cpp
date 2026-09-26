// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#include <cassert>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <cstdarg>
#include <gdfx/math/Bezier.hpp>
#include <gdfx/graphics/canvas/Canvas.hpp>
#include <gdfx/graphics/canvas/Image.hpp>

namespace gdfx {

Canvas::Canvas() :
    buffer(nullptr),
    width(0),
    height(0),
    clipX(0),
    clipY(0),
    clipX2(0),
    clipY2(0),
    lineClipper(),
    palette()
{
}

Canvas::~Canvas()
{
    destroy();
}

void Canvas::create(int w, int h)
{
    destroy();

    buffer = new uint8_t[w * h + 1]; // +1 for PCX being stupid
    width = w;
    height = h;

    palette.resize(256);
    palette.clear();

    setClipRect(0, 0, width, height);
    clear();
}

void Canvas::destroy()
{
    if (buffer) {
        delete[] buffer;
        buffer = nullptr;
    }

    width = height = 0;
}

uint8_t *Canvas::getBuffer()
{
    return buffer;
}

Palette &Canvas::getPalette()
{
    return palette;
}

int Canvas::getWidth() const
{
    return width;
}

int Canvas::getHeight() const
{
    return height;
}

void Canvas::setClipRect(int x, int y, int w, int h)
{
    clipX = x;
    clipY = y;
    clipX2 = x + w - 1;
    clipY2 = y + h - 1;

    clipRect.set(x, y, w, h);

    lineClipper.setClipRect(x, y, w, h);
}

const Rectangle& Canvas::getClipRect() const
{
    return clipRect;
}

void Canvas::clear()
{
    clearToColor(0);
}

void Canvas::clearToColor(int color)
{
    std::memset(buffer, color, width * height * sizeof(uint8_t));
}

void Canvas::drawPixel(int x, int y, int color)
{
    if (x < clipX || x > clipX2 || y < clipY || y > clipY2)
        return;

    buffer[y * width + x] = (uint8_t)color;
}

void Canvas::drawLine(int x1, int y1, int x2, int y2, int color)
{
    if (y1 == y2)
        drawHLine(x1, y1, x2, color);
    else if (x1 == x2)
        drawVLine(x1, y1, y2, color);
    else {
        if (lineClipper.clip(x1, y1, x2, y2))
            return;
        drawBresenhamLineNoClip(x1, y1, x2, y2, color);
    }
}

void Canvas::drawHLine(int x1, int y1, int x2, int color)
{
    if (x1 > x2)
        std::swap(x1, x2);

    if (x1 <= clipX2 && x2 >= clipX && y1 >= clipY && y1 <= clipY2) {
        x1 = std::max(x1, clipX);
        x2 = std::min(x2, clipX2);
        std::memset(buffer + y1 * width + x1, color, (x2 - x1) + 1);
    }
}

void Canvas::drawVLine(int x1, int y1, int y2, int color)
{
    if (y1 > y2)
        std::swap(y1, y2);

    if (y1 <= clipY2 && y2 >= clipY && x1 >= clipX && x1 <= clipX2) {
        y1 = std::max(y1, clipY);
        y2 = std::min(y2, clipY2);
        for (int y = y1; y <= y2; y++)
            buffer[y * width + x1] = (uint8_t)color;
    }
}

void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color)
{
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x3, y3, color);
	drawLine(x3, y3, x1, y1, color);
}

// From "Building a 3D Game Engine in C++"
void Canvas::drawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color)
{
	// sort our vertices from top to bottom
	if (y3 < y1) {
		std::swap(y3, y1);
		std::swap(x3, x1);
	}
	if (y2 < y1) {
		std::swap(y2, y1);
		std::swap(x2, x1);
	}
	if (y3 < y2) {
		std::swap(y3, y2);
		std::swap(x3, x2);
	}

	// if the triangle has no horizontal edges then split it into two triangles that do
	if (y2 != y1 && y3 != y1 && y3 != y2) {
		int xn;
		float ratio;

		// determine new x value
		ratio = (float)(y2 - y1) / (float)(y3 - y1);
		xn = (int)(((float)(x3 - x1) * ratio) + (float)x1);

		// Draw the two subtriangles
		if (xn < x2) {
			drawFilledFBTriangle(x1, y1, xn, y2, x2, y2, color);
			drawFilledFBTriangle(x3, y3, x2, y2, xn, y2, color);
		}
		else {
			drawFilledFBTriangle(x1, y1, x2, y2, xn, y2, color);
			drawFilledFBTriangle(x3, y3, x2, y2, xn, y2, color);
		}
	}
	// top is flat
	else if (y1 == y2) {
		if (x1 < x2)
			drawFilledFBTriangle(x3, y3, x2, y2, x1, y2, color);
		else
			drawFilledFBTriangle(x3, y3, x1, y2, x2, y2, color);
	}
	// bottom is flat
	else if (y2 == y3) {
		if (x2 < x3)
			drawFilledFBTriangle(x1, y1, x2, y2, x3, y2, color);
		else
			drawFilledFBTriangle(x1, y1, x3, y2, x2, y2, color);
	}
}

// From "Building a 3D Game Engine in C++"
void Canvas::drawFilledFBTriangle(int x1, int y1, int x2, int y23, int x3, int, int color)
{
	float dx_left, dx_right;
	float curx_left, curx_right;
	float height = (float)(y23 - y1);

	// make sure y23 - y1 != 0
	if ((int)height == 0)
		return;

	// determine the change in X (ie, slope) for left and right sides
	dx_left = (x2 - x1) / height;
	dx_right = (x3 - x1) / height;

	curx_left = curx_right = (float)x1;
	// Draw each scan line from top to bottom
	if (y1 < y23) {
		for (int y = y1; y <= y23; y++) {
			drawHLine((int)curx_left, y, (int)curx_right, color);
			// adjust X coordinates
			curx_left += dx_left;
			curx_right += dx_right;
		}
	}
	else {
		for (int y = y1; y >= y23; y--) {
			drawHLine((int)curx_left, y, (int)curx_right, color);
			// adjust X coordinates
			curx_left -= dx_left;
			curx_right -= dx_right;
		}
	}
}

void Canvas::drawRectangle(int x, int y, int w, int h, int color)
{
	// top, bottom
	drawHLine(x, y, x + w - 1, color);
	drawHLine(x, y + h - 1, x + w - 1, color);

	// left, right
	drawVLine(x, y, y + h - 1, color);
	drawVLine(x + w - 1, y, y + h - 1, color);
}

void Canvas::drawFilledRectangle(int x, int y, int w, int h, int color)
{
	int x2 = x + w - 1;
	int y2 = y + h - 1;

	if (x <= clipX2 && x2 >= clipX && y <= clipY2 && y2 >= clipY) {
		y = std::max(y, clipY);
		y2 = std::min(y2, clipY2);
		x = std::max(x, clipX);
		x2 = std::min(x2, clipX2);
		for (int i = y; i <= y2; i++) {
			drawHLine(x, i, x2, color);
		}
	}
}

void Canvas::drawQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int color)
{
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x3, y3, color);
	drawLine(x3, y3, x4, y4, color);
	drawLine(x4, y4, x1, y1, color);
}

void Canvas::drawFilledQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int color)
{
	drawFilledTriangle(x1, y1, x2, y2, x3, y3, color);
	drawFilledTriangle(x1, y1, x3, y3, x4, y4, color);
}

void Canvas::drawCircle(int xOrigin, int yOrigin, int radius, int color)
{
    int x = 0;
    int y = radius;
    int d = 1 - radius;
    int deltaE = 3;
    int deltaSE = -2 * radius + 5;

    drawCirclePoints(xOrigin, yOrigin, x, y, color);

    while (y > x) {
        if (d < 0) {
            d += deltaE; // select E
            deltaE += 2;
            deltaSE += 2;
        }
        else {
            d += deltaSE; // select SE
            deltaSE += 2;
            deltaE += 4;
            y--;
        }
        x++;
        drawCirclePoints(xOrigin, yOrigin, x, y, color);
    }
}

void Canvas::drawCirclePoints(int xOrigin, int yOrigin, int x, int y, int color)
{
    drawPixel(xOrigin + x, yOrigin + y, color);
    drawPixel(xOrigin + y, yOrigin + x, color);
    drawPixel(xOrigin + y, yOrigin - x, color);
    drawPixel(xOrigin + x, yOrigin - y, color);
    drawPixel(xOrigin - x, yOrigin - y, color);
    drawPixel(xOrigin - y, yOrigin - x, color);
    drawPixel(xOrigin - y, yOrigin + x, color);
    drawPixel(xOrigin - x, yOrigin + y, color);
}

void Canvas::drawCanvas(Canvas& canvas, int sx, int sy, int sw, int sh, int dx, int dy)
{
    Rectangle r1(dx, dy, sw, sh);
    Rectangle dstRect;

    if (!Rectangle::intersect(r1, getClipRect(), dstRect))
        return;
    if (dstRect.w == 0 || dstRect.h == 0)
        return;

    const int w = dstRect.w;
    const int h = dstRect.h;

    const int srcNextLine = canvas.getWidth() - w;
    const int dstNextLine = getWidth() - w;

    uint8_t *src = canvas.getBuffer() + sy * canvas.getWidth() + sx;
    uint8_t *dst = getBuffer() + dy * getWidth() + dx;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            *dst++ = *src++;
        }
        dst += dstNextLine;
        src += srcNextLine;
    }
}

void Canvas::drawMaskedCanvas(Canvas& canvas, int sx, int sy, int sw, int sh, int dx, int dy)
{
    Rectangle r1(dx, dy, sw, sh);
    Rectangle dstRect;

    if (!Rectangle::intersect(r1, getClipRect(), dstRect))
        return;
    if (dstRect.w == 0 || dstRect.h == 0)
        return;

    const int w = dstRect.w;
    const int h = dstRect.h;

    const int srcNextLine = canvas.getWidth() - w;
    const int dstNextLine = getWidth() - w;

    uint8_t *src = canvas.getBuffer() + sy * canvas.getWidth() + sx;
    uint8_t *dst = getBuffer() + dy * getWidth() + dx;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            uint8_t srcColor = *src++;
            if (srcColor)
                *dst = srcColor;
            dst++;
        }
        dst += dstNextLine;
        src += srcNextLine;
    }
}

void Canvas::drawMaskedColorModCanvas(Canvas& canvas, int sx, int sy, int sw, int sh, int dx, int dy, int color)
{
    Rectangle r1(dx, dy, sw, sh);
    Rectangle dstRect;

    if (!Rectangle::intersect(r1, getClipRect(), dstRect))
        return;
    if (dstRect.w == 0 || dstRect.h == 0)
        return;

    const int w = dstRect.w;
    const int h = dstRect.h;

    const int srcNextLine = canvas.getWidth() - w;
    const int dstNextLine = getWidth() - w;

    uint8_t *src = canvas.getBuffer() + sy * canvas.getWidth() + sx;
    uint8_t *dst = getBuffer() + dy * getWidth() + dx;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (*src++)
                *dst = color;
            dst++;
        }
        dst += dstNextLine;
        src += srcNextLine;
    }
}

void Canvas::drawImage(Image *image, int sx, int sy, int sw, int sh, int dx, int dy)
{
    drawCanvas(*image, sx, sy, sw, sh, dx, dy);
}

void Canvas::drawMaskedImage(Image *image, int sx, int sy, int sw, int sh, int dx, int dy)
{
    drawMaskedCanvas(*image, sx, sy, sw, sh, dx, dy);
}

void Canvas::drawMaskedColorModImage(Image *image, int sx, int sy, int sw, int sh, int dx, int dy, int color)
{
    drawMaskedColorModCanvas(*image, sx, sy, sw, sh, dx, dy, color);
}

void Canvas::drawTextEx(ImageFont *font, int x, int y, int color, ImageFont::HorizontalAlign hAlign, ImageFont::VerticalAlign vAlign, int charsToDraw, const char* text)
{
	int len = (int)std::strlen(text);
	if (charsToDraw == -1 || charsToDraw > len)
		charsToDraw = len;

	int textWidthInPixels = len * font->getSize();
	if (hAlign == ImageFont::HorizontalAlign::CENTER)
		x -= textWidthInPixels / 2;
	else if (hAlign == ImageFont::HorizontalAlign::RIGHT)
		x -= textWidthInPixels;

	if (vAlign == ImageFont::VerticalAlign::CENTER)
		y -= font->getSize() / 2;
	else if (vAlign == ImageFont::VerticalAlign::BOTTOM)
		y -= font->getSize();

	for (int i = 0; i < charsToDraw; i++) {
		const ImageFont::Glyph& g = font->getGlyph(text[i] - 32);
	    drawMaskedColorModImage(font->getImage(), g.srcRect.x, g.srcRect.y, g.srcRect.w, g.srcRect.h, x, y, color);
		x += font->getSize();
	}
}

void Canvas::drawText(ImageFont *font, int x, int y, int color, const char* fmt, ...)
{
	char buffer[4096];
	va_list args;

	va_start(args, fmt);
#if defined _WIN32
	vsprintf_s(buffer, 4096, fmt, args);
#else
	vsprintf(buffer, fmt, args);
#endif
	va_end(args);

	std::istringstream text(buffer);
	std::string line;

	while (std::getline(text, line, '\n')) {
		drawTextEx(font, x, y, color, ImageFont::HorizontalAlign::LEFT, ImageFont::VerticalAlign::TOP, -1, line.c_str());
		y += font->getSize();
		x = 0;
	}
}

void Canvas::drawTextCentered(ImageFont *font, int x, int y, int color, const char* fmt, ...)
{
	char buffer[4096];
	va_list args;

	va_start(args, fmt);
#if defined _WIN32
	vsprintf_s(buffer, 4096, fmt, args);
#else
	vsprintf(buffer, fmt, args);
#endif
	va_end(args);

	std::istringstream text(buffer);
	std::string line;

	while (std::getline(text, line, '\n')) {
		drawTextEx(font, x, y, color, ImageFont::HorizontalAlign::CENTER, ImageFont::VerticalAlign::TOP, -1, line.c_str());
		y += font->getSize();
		x = 0;
	}
}

void Canvas::drawTextRight(ImageFont *font, int x, int y, int color, const char* fmt, ...)
{
	char buffer[4096];
	va_list args;

	va_start(args, fmt);
#if defined _WIN32
	vsprintf_s(buffer, 4096, fmt, args);
#else
	vsprintf(buffer, fmt, args);
#endif
	va_end(args);

	std::istringstream text(buffer);
	std::string line;

	while (std::getline(text, line, '\n')) {
		drawTextEx(font, x, y, color, ImageFont::HorizontalAlign::RIGHT, ImageFont::VerticalAlign::TOP, -1, line.c_str());
		y += font->getSize();
		x = 0;
	}
}

/**
* Draws a bezier curve using the specified control points, the desired number of points, and given color.
*/
void Canvas::drawBezier(Vector2 controlPoints[4], int numDesiredPoints, int color)
{
    assert(numDesiredPoints >= 2);

    std::vector<Vector2> points;
    Bezier::calcCurve(controlPoints, numDesiredPoints, points);

    for (int i = 0; i < points.size() - 1; i++) {
        const Vector2 pt1 = points[i];
        const Vector2 pt2 = points[i + 1];
        drawLine(pt1.x, pt1.y, pt2.x, pt2.y, color);
    }
}

/**
* Draws a circular arc between 0 and PI angles (up to a half circle).
*
* @param center Center point of the circle arc
* @param radius The circle radius
* @param angle Angle between 0 and PI radians
*/
void Canvas::drawCircleArc(const Vector2& center, int radius, float angle, int numDesiredPoints, int color)
{
    assert(numDesiredPoints >= 2);

    std::vector<Vector2> points;
    Bezier::calcCircularArc(center, (float)radius, angle, numDesiredPoints, points);

    for (int i = 0; i < points.size() - 1; i++) {
        const Vector2 pt1 = points[i];
        const Vector2 pt2 = points[i + 1];
        drawLine(pt1.x, pt1.y, pt2.x, pt2.y, color);
    }
}

void Canvas::drawFilledCircleArc(const Vector2& center, int radius, float angle, int numDesiredPoints, int color, bool includeCenterPoint)
{
    assert(numDesiredPoints >= 2);

    std::vector<Vector2> points;
    Bezier::calcCircularArc(center, (float)radius, angle, numDesiredPoints, points);

    if (includeCenterPoint)
        points.push_back(center);

    Polygon2D poly;
    poly.numVertices = points.size();
    for (int i = 0; i < points.size(); i++) {
        poly.vertices[i] = points[i];
    }

    std::vector<Triangle2D> triangles;
    triangulate2D(poly, triangles);

    for (int i = 0; i < triangles.size(); i++) {
        const Vector2& pt1 = triangles[i].vertices[0];
        const Vector2& pt2 = triangles[i].vertices[1];
        const Vector2& pt3 = triangles[i].vertices[2];
        drawFilledTriangle(
            pt1.x, pt1.y,
            pt2.x, pt2.y,
            pt3.x, pt3.y,
            color
        );
    }
}

void Canvas::drawPixelNoClip(int x, int y, int color)
{
    buffer[y * width + x] = (uint8_t)color;
}

void Canvas::drawLineNoClip(int x1, int y1, int x2, int y2, int color)
{
    if (y1 == y2)
        drawHLineNoClip(x1, y1, x2, color);
    else if (x1 == x2)
        drawVLineNoClip(x1, y1, y2, color);
    else {
        drawBresenhamLineNoClip(x1, y1, x2, y2, color);
    }
}

void Canvas::drawHLineNoClip(int x1, int y1, int x2, int color)
{
    if (x1 > x2)
        std::swap(x1, x2);

    std::memset(buffer + y1 * width + x1, color, (x2 - x1) + 1);
}

void Canvas::drawVLineNoClip(int x1, int y1, int y2, int color)
{
    if (y1 > y2)
        std::swap(y1, y2);

    for (int y = y1; y <= y2; y++)
        buffer[y * width + x1] = (uint8_t)color;
}

/* |DeltaX|>=DeltaY */
void Canvas::drawBresenhamLineOctant0NoClip(int x0, int y0, int dx, int dy, int direction, int color)
{
    int dy_x2;
    int dy_x2_minus_dx_x2;
    int error;
    uint8_t *p;

    dy_x2 = dy * 2;
    dy_x2_minus_dx_x2 = dy_x2 - dx * 2;
    error = dy_x2 - dx;

    p = buffer + y0 * width + x0;
    *p = color;
    while (dx--) {
        if (error >= 0) {
            y0++;
            p += width;
            error += dy_x2_minus_dx_x2;
        }
        else {
            error += dy_x2;
        }
        x0 += direction;
        p += direction;
        *p = color;
    }
}

/* |DeltaX|<DeltaY */
void Canvas::drawBresenhamLineOctant1NoClip(int x0, int y0, int dx, int dy, int direction, int color)
{
    int dx_x2;
    int dx_x2_minus_dy_x2;
    int error;
    uint8_t *p;

    dx_x2 = dx * 2;
    dx_x2_minus_dy_x2 = dx_x2 - dy * 2;
    error = dx_x2 - dy;

    p = buffer + y0 * width + x0;
    *p = color;
    while (dy--) {
        if (error >= 0) {
            x0++;
            p += direction;
            error += dx_x2_minus_dy_x2;
        }
        else {
            error += dx_x2;
        }
        y0++;
        p += width;
        *p = color;
    }
}

void Canvas::drawBresenhamLineNoClip(int x0, int y0, int x1, int y1, int color)
{
    int dx, dy;

    /* make dy always > 0, so only octant 0-3 are needed */
    if (y0 > y1) {
        std::swap(y0, y1);
        std::swap(x0, x1);
    }

    dx = x1 - x0;
    dy = y1 - y0;

    if (dx > 0) {
        if (dx > dy) {
            drawBresenhamLineOctant0NoClip(x0, y0, dx, dy, 1, color);
        }
        else {
            drawBresenhamLineOctant1NoClip(x0, y0, dx, dy, 1, color);
        }
    }
    else {
        dx = -dx;
        if (dx > dy) {
            drawBresenhamLineOctant0NoClip(x0, y0, dx, dy, -1, color);
        }
        else {
            drawBresenhamLineOctant1NoClip(x0, y0, dx, dy, -1, color);
        }
    }
}

void Canvas::triangulate2D(const Polygon2D& poly, std::vector<Triangle2D>& triangles)
{
    triangles.clear();

    for (int i = 0; i < poly.numVertices - 2; i++) {
        int v0 = 0;
        int v1 = i + 1;
        int v2 = i + 2;

        Triangle2D tri;

        tri.vertices[0] = poly.vertices[v0];
        tri.vertices[1] = poly.vertices[v1];
        tri.vertices[2] = poly.vertices[v2];

        triangles.push_back(tri);
    }
}

} // gdfx


