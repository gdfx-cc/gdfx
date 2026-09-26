//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#ifndef GDFX_GAME_CANVASGAME_HPP
#define GDFX_GAME_CANVASGAME_HPP

#include <gdfx/game/Game.hpp>
#include <gdfx/graphics/canvas/Canvas.hpp>
#include <gdfx/graphics/canvas/Palette.hpp>
#include <gdfx/graphics/canvas/Rasterizer3D.hpp>

namespace gdfx {

class CanvasGame : public Game {
public:
    CanvasGame(const char *name, const char *identifier, const char *version, int width, int height, int fps = DEFAULT_FRAMES_PER_SECOND);
    virtual ~CanvasGame();

    virtual void draw(Canvas& canvas, Rasterizer3D& rasterizer) {}
    virtual void draw(Graphics& g) override final;

    Canvas& getCanvas() { return canvas; }
    Palette& getCanvasPalette() { return canvasPalette; }
    Rasterizer3D& getRasterizer() { return rasterizer; }

protected:
    virtual void initializeCanvasPalette();

private:
    Canvas canvas;
    Palette canvasPalette;
    Rasterizer3D rasterizer;
};

} // gdfx

#endif // GDFX_GAME_CANVASGAME_HPP

