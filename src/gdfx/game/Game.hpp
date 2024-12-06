//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_GAME_GAME_HPP__
#define __GDFX_GAME_GAME_HPP__

#include <iostream>
#include <gdfx/platform/SDL3.hpp>

namespace gdfx {

class Game {
public:
    static Game *createGame(); // implement in derived class cpp via CREATE_GAME(MyGame);

    Game(const char *name, const char *identifier, const char *version, int width, int height) :
        gameName(name),
        gameIdentifier(identifier),
        gameVersion(version),
        gameWidth(width),
        gameHeight(height)
    {
        SDL_SetAppMetadata(name, version, identifier);

        if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_GAMEPAD))
            std::cout << "Successfully initialized SDL" << std::endl;
        else
            std::cout << "Error initializing SDL" << std::endl;
    }

private:
    std::string gameName;
    std::string gameIdentifier;
    std::string gameVersion;
    int gameWidth;
    int gameHeight;
};

} // gdfx

#define CREATE_GAME(MyClass) gdfx::Game *gdfx::Game::createGame() { return new MyClass(); }

#endif // __GDFX_GAME_GAME_HPP__
