//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/game/Game.hpp>

namespace gdfx {

Game::Game(const char *name, const char *identifier, const char *version, int width, int height) :
    graphics(),
    appName(name),
    appIdentifier(identifier),
    appVersion(version),
    width(width),
    height(height)
{
    if (!SDL_SetAppMetadata(name, version, identifier))
        throw SDLException();

    if (!SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_GAMEPAD))
        throw SDLException();

    graphics.create(name, width, height);
}

Game::~Game()
{
}

void Game::iterate()
{
    update();
    graphics.begin();
    draw(graphics);
    graphics.end();
}

} // gdfx
