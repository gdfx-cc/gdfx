//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
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
    msPerFrame(1000 / framesPerSecond),
    lastTime(0)
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
    input.poll((uint32_t)msPerFrame);

    uint64_t currentTime = SDL_GetTicks();
    if (currentTime - lastTime >= (uint64_t)msPerFrame) {
        float delta = msPerFrame / 1000.0f;
        
        update(delta);

        graphics.begin();
        draw(graphics);
        graphics.end();

        lastTime = SDL_GetTicks();
    }
}

} // gdfx
