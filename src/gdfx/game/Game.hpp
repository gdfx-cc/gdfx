//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_GAME_GAME_HPP__
#define __GDFX_GAME_GAME_HPP__

#include <iostream>
#include <string>
#include <gdfx/platform/SDL3.hpp>
#include <gdfx/platform/SDLException.hpp>

namespace gdfx {

class Game {
public:
    static Game *createGame(); // implement in derived class cpp via CREATE_GAME(MyGame);

    Game(const char *name, const char *identifier, const char *version, int width, int height) :
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
    }

    virtual void update() {}
    virtual void draw() {}

    const std::string& getAppName() const { return appName; }
    const std::string& getAppIdentifier() const { return appIdentifier; }
    const std::string& getAppVersion() const { return appVersion; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    std::string appName;
    std::string appIdentifier;
    std::string appVersion;
    int width;
    int height;
};

} // gdfx

#define CREATE_GAME(MyClass) gdfx::Game *gdfx::Game::createGame() { return new MyClass(); }

#endif // __GDFX_GAME_GAME_HPP__
