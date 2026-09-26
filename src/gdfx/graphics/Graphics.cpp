//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#include <cmath>
#include <vector>
#include <sstream>
#include <gdfx/platform/SDLException.hpp>
#include <gdfx/graphics/Graphics.hpp>
#include <gdfx/math/Math.hpp>

namespace gdfx {

Graphics::Graphics() :
    window(nullptr),
    renderer(nullptr),
    renderTarget(nullptr),
    canvasTexture(nullptr),
    canvasTextureWidth(0),
    canvasTextureHeight(0),
    width(0),
    height(0)
{
}

Graphics::~Graphics()
{
    destroy();
}

void Graphics::create(const char *title, int w, int h)
{
    if (!SDL_CreateWindowAndRenderer(title, w, h, SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED, &window, &renderer))
        throw SDLException();
    SDL_SetRenderVSync(renderer, 1);

    //SDL_SetRenderLogicalPresentation(renderer, w, h, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderLogicalPresentation(renderer, w, h, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

    renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureScaleMode(renderTarget, SDL_SCALEMODE_NEAREST);

    width = w;
    height = h;
}

void Graphics::destroy()
{
    if (canvasTexture) {
        SDL_DestroyTexture(canvasTexture);
        canvasTexture = nullptr;
    }
    canvasTextureWidth = canvasTextureHeight = 0;

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

void Graphics::setColor(const Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

void Graphics::clear()
{
    SDL_RenderClear(renderer);
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

void Graphics::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

void Graphics::drawCircle(int x, int y, int radius)
{
    const int diameter = radius * 2;
    int dx = radius - 1;
    int dy = 0;
    int tx = 1;
    int ty = 1;
    int error = tx - diameter;
    std::vector<SDL_FPoint> points;
    while (dx >= dy) {
        points.emplace_back(SDL_FPoint{ (float)(x + dx), (float)(y - dy) });
        points.emplace_back(SDL_FPoint{ (float)(x + dx), (float)(y + dy) });
        points.emplace_back(SDL_FPoint{ (float)(x - dx), (float)(y - dy) });
        points.emplace_back(SDL_FPoint{ (float)(x - dx), (float)(y + dy) });
        points.emplace_back(SDL_FPoint{ (float)(x + dy), (float)(y - dx) });
        points.emplace_back(SDL_FPoint{ (float)(x + dy), (float)(y + dx) });
        points.emplace_back(SDL_FPoint{ (float)(x - dy), (float)(y - dx) });
        points.emplace_back(SDL_FPoint{ (float)(x - dy), (float)(y + dx) });

        if (error <= 0) {
            dy++;
            error += ty;
            ty += 2;
        }
        if (error > 0) {
            dx--;
            tx += 2;
            error += (tx - diameter);
        }
    }

    if (!points.empty())
        SDL_RenderPoints(renderer, points.data(), points.size());
}

void Graphics::drawTexture(Texture& texture, int x, int y)
{
    SDL_FRect dst{ (float)x, (float)y, (float)texture.getWidth(), (float)texture.getHeight() };
    SDL_RenderTexture(renderer, texture.getTexture(), nullptr, &dst);
}

void Graphics::drawTexture(Texture& texture, int sx, int sy, int sw, int sh, int dx, int dy)
{
    SDL_FRect src{ (float)sx, (float)sy, (float)sw, (float)sh };
    SDL_FRect dst{ (float)dx, (float)dy, (float)sw, (float)sh };
    SDL_RenderTexture(renderer, texture.getTexture(), &src, &dst);
}

void Graphics::drawTextureRegion(TextureRegion& textureRegion, int x, int y)
{
    SDL_FRect src{ (float)textureRegion.getX(), (float)textureRegion.getY(), (float)textureRegion.getWidth(), (float)textureRegion.getHeight() };
    SDL_FRect dst{ (float)x, (float)y, (float)textureRegion.getWidth(), (float)textureRegion.getHeight() };
    SDL_RenderTexture(renderer, textureRegion.getTexture()->getTexture(), &src, &dst);
}

void Graphics::drawSprite(Sprite& sprite, int x, int y)
{
	drawTextureRegion(sprite.getFrameTextureRegion(), x, y);
}

void Graphics::drawCanvas(Canvas& canvas, float x, float y)
{
    drawCanvas(canvas, canvas.getPalette(), x, y);
}

void Graphics::drawCanvas(Canvas& canvas, const Palette& palette, float x, float y)
{
    updateCanvasTexture(canvas, palette);

    SDL_FRect dst{ x, y, (float)canvas.getWidth(), (float)canvas.getHeight() };
    SDL_RenderTexture(renderer, canvasTexture, nullptr, &dst);
}

void Graphics::drawFilledRect(int x, int y, int w, int h)
{
    SDL_FRect rect{ (float)x, (float)y, (float)w, (float)h };
    SDL_RenderFillRect(renderer, &rect);
}

void Graphics::drawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    const int numVerts = 3;
    SDL_Vertex verts[numVerts];

    float r, g, b, a;
    SDL_GetRenderDrawColorFloat(renderer, &r, &g, &b, &a);

    verts[0].position.x = (float)x1;
    verts[0].position.y = (float)y1;
    verts[0].color.r = r;
    verts[0].color.g = g;
    verts[0].color.b = b;
    verts[0].color.a = a;

    verts[1].position.x = (float)x2;
    verts[1].position.y = (float)y2;
    verts[1].color.r = r;
    verts[1].color.g = g;
    verts[1].color.b = b;
    verts[1].color.a = a;

    verts[2].position.x = (float)x3;
    verts[2].position.y = (float)y3;
    verts[2].color.r = r;
    verts[2].color.g = g;
    verts[2].color.b = b;
    verts[2].color.a = a;

    SDL_RenderGeometry(renderer, nullptr, verts, numVerts, nullptr, 0);
}

void Graphics::drawFilledCircle(int x, int y, int radius)
{
    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;

    float r, g, b, a;
    SDL_GetRenderDrawColorFloat(renderer, &r, &g, &b, &a);

    SDL_Vertex center;

    center.position.x = (float)x;
    center.position.y = (float)y;
    center.color.r = r;
    center.color.g = g;
    center.color.b = b;
    center.color.a = a;

    vertices.push_back(center);

    for (float phi = 0; phi < 2*Math::PI; phi += Math::PI/10) {
        SDL_Vertex v;

        v.position.x = x + (float)radius * cos(phi);
        v.position.y = y + (float)radius * sin(phi);
        v.color.r = r;
        v.color.g = g;
        v.color.b = b;
        v.color.a = a;
        
        vertices.emplace_back(v);
    }

    for (int i = 1; i < vertices.size(); i++) {
        indices.push_back(0);
        indices.push_back(i);
        if (i + 1 < vertices.size())
            indices.push_back(i+1);
        else
            indices.push_back(1);
    }

    SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(),
        indices.data(), indices.size());
}

void Graphics::drawTextEx(Font *font, int x, int y, Font::HorizontalAlign hAlign, Font::VerticalAlign vAlign, int charsToDraw, const char* text)
{
	int len = (int)std::strlen(text);
	if (charsToDraw == -1 || charsToDraw > len)
		charsToDraw = len;

	int textWidthInPixels = len * font->getSize();
	if (hAlign == Font::HorizontalAlign::CENTER)
		x -= textWidthInPixels / 2;
	else if (hAlign == Font::HorizontalAlign::RIGHT)
		x -= textWidthInPixels;

	if (vAlign == Font::VerticalAlign::CENTER)
		y -= font->getSize() / 2;
	else if (vAlign == Font::VerticalAlign::BOTTOM)
		y -= font->getSize();

    // apply the current draw color as the texture mod color
    float r, g, b, a;
    SDL_GetRenderDrawColorFloat(renderer, &r, &g, &b, &a);
    SDL_SetTextureColorModFloat(font->getTexture()->getTexture(), r, g, b);

	for (int i = 0; i < charsToDraw; i++) {
		const Font::Glyph& g = font->getGlyph(text[i] - 32);
	    drawTexture(*font->getTexture(), g.srcRect.x, g.srcRect.y, g.srcRect.w, g.srcRect.h, x, y);
		x += font->getSize();
	}
}

void Graphics::drawText(Font *font, int x, int y, const char* fmt, ...)
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
		drawTextEx(font, x, y, Font::HorizontalAlign::LEFT, Font::VerticalAlign::TOP, -1, line.c_str());
		y += font->getSize();
		x = 0;
	}
}

void Graphics::drawTextCentered(Font *font, int x, int y, const char* fmt, ...)
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
		drawTextEx(font, x, y, Font::HorizontalAlign::CENTER, Font::VerticalAlign::TOP, -1, line.c_str());
		y += font->getSize();
		x = 0;
	}
}

void Graphics::drawTextRight(Font *font, int x, int y, const char* fmt, ...)
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
		drawTextEx(font, x, y, Font::HorizontalAlign::RIGHT, Font::VerticalAlign::TOP, -1, line.c_str());
		y += font->getSize();
		x = 0;
	}
}

void Graphics::updateCanvasTexture(Canvas& canvas, const Palette& palette)
{
    const int canvasWidth = canvas.getWidth();
    const int canvasHeight = canvas.getHeight();

    if (canvasTextureWidth != canvasWidth || canvasTextureHeight != canvasHeight) {
        if (canvasTexture) {
            SDL_DestroyTexture(canvasTexture);
            canvasTexture = nullptr;
        }

        canvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, canvasWidth, canvasHeight);
        if (!canvasTexture)
            throw SDLException();

        SDL_SetTextureScaleMode(canvasTexture, SDL_SCALEMODE_NEAREST);
        canvasTextureWidth = canvasWidth;
        canvasTextureHeight = canvasHeight;
    }

    uint8_t *src = canvas.getBuffer();
    uint8_t *pixels = nullptr;
    int pitch = 0;

    if (!SDL_LockTexture(canvasTexture, nullptr, (void**)&pixels, &pitch))
        throw SDLException();

    for (int y = 0; y < canvasHeight; y++) {
        uint32_t *dst = (uint32_t *)(pixels + y * pitch);
        for (int x = 0; x < canvasWidth; x++) {
            *dst++ = palette.getRGBA(*src++);
        }
    }

    SDL_UnlockTexture(canvasTexture);
}

} // gdfx
