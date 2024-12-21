//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <cmath>
#include <vector>
#include <gdfx/platform/SDLException.hpp>
#include <gdfx/graphics/Graphics.hpp>
#include <gdfx/math/Math.hpp>

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
    destroy();
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

void Graphics::destroy()
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
    static std::vector<SDL_FPoint> points;

    points.clear();
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

    SDL_RenderPoints(renderer, &points[0], points.size());
}

void Graphics::drawImage(Image& img, int x, int y)
{
    SDL_FRect dst{ (float)x, (float)y, (float)img.getWidth(), (float)img.getHeight() };
    SDL_RenderTexture(renderer, img.getTexture(), nullptr, &dst);
}

void Graphics::drawImageRegion(ImageRegion& imageRegion, int x, int y)
{
    SDL_FRect src{ (float)imageRegion.getX(), (float)imageRegion.getY(), (float)imageRegion.getWidth(), (float)imageRegion.getHeight() };
    SDL_FRect dst{ (float)x, (float)y, (float)imageRegion.getWidth(), (float)imageRegion.getHeight() };
    SDL_RenderTexture(renderer, imageRegion.getImage()->getTexture(), &src, &dst);
}

void Graphics::drawSprite(Sprite& sprite, int x, int y)
{
	drawImageRegion(sprite.getFrameImageRegion(), x, y);
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

} // gdfx
