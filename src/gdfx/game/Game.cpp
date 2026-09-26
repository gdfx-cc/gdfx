//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/game/Game.hpp>

namespace gdfx {

Game::Game(const char *name, const char *identifier, const char *version, int width, int height, int fps) :
    graphics(),
    audio(),
    input(),
    content(),
    appName(name),
    appIdentifier(identifier),
    appVersion(version),
    width(width),
    height(height),
    framesPerSecond(fps),
    targetDelta(1.0 / framesPerSecond),
    lastTimeNS(0)
{
    if (!SDL_SetAppMetadata(name, version, identifier))
        throw SDLException();

    if (!SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_GAMEPAD))
        throw SDLException();

    graphics.create(name, width, height);
    content.setGraphics(&graphics);

    audio.create();
}

Game::~Game()
{
}

void Game::iterate()
{
    uint64_t currentTimeNS = SDL_GetTicksNS();
    if (lastTimeNS == 0) {
        lastTimeNS = currentTimeNS;
    }

    double delta = (currentTimeNS - lastTimeNS) / 1000000000.0;
    if (delta > 0.25) {
        delta = targetDelta;
    }
    lastTimeNS = currentTimeNS;

    input.poll((uint32_t)(delta * 1000.0));
    update((float)delta);

    graphics.begin();
    draw(graphics);
    graphics.end();
}

} // gdfx
