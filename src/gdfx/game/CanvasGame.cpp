//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#include <algorithm>
#include <gdfx/game/CanvasGame.hpp>

namespace gdfx {

CanvasGame::CanvasGame(const char *name, const char *identifier, const char *version, int width, int height, int fps) :
    Game(name, identifier, version, width, height, fps),
    canvas(),
    canvasPalette(),
    rasterizer()
{
    canvas.create(width, height);
    initializeCanvasPalette();

    rasterizer.create(&canvas);
    rasterizer.renderSetLight(0.0f, 1.0f, -0.5f);
}

CanvasGame::~CanvasGame()
{
}

void CanvasGame::draw(Graphics& g)
{
    canvas.clear();
    draw(canvas, rasterizer);
    g.drawCanvas(canvas, canvasPalette);
}

void CanvasGame::initializeCanvasPalette()
{
    canvasPalette.resize(16 + 16 * 9);

    for (int i = 0; i < 16; i++) {
        canvasPalette.setColor(i, Color::C64_PALETTE[i]);
        canvas.getPalette().setColor(i, Color::C64_PALETTE[i]);
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 16; j++) {
            Color c = Color::C64_PALETTE[j];
            int shift = i - 4;
            int scale = 20;

            int r = c.r + shift * scale;
            int g = c.g + shift * scale;
            int b = c.b + shift * scale;

            c.r = std::min(255, std::max(r, 0));
            c.g = std::min(255, std::max(g, 0));
            c.b = std::min(255, std::max(b, 0));
            c.a = 255;

            canvasPalette.setColor(16 + i * 16 + j, c);
        }
    }
}

} // gdfx
