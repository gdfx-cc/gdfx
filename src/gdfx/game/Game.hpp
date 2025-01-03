//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_GAME_GAME_HPP__
#define __GDFX_GAME_GAME_HPP__

#include <string>
#include <gdfx/platform/SDL3.hpp>
#include <gdfx/platform/SDLException.hpp>
#include <gdfx/graphics/Graphics.hpp>
#include <gdfx/audio/Audio.hpp>
#include <gdfx/input/Input.hpp>
#include <gdfx/content/ContentManager.hpp>

namespace gdfx {

class Game {
public:
    static Game *createGame(); // implement in derived class cpp via CREATE_GAME(MyGame);
    static constexpr int DEFAULT_FRAMES_PER_SECOND = 60;

    Game(const char *name, const char *identifier, const char *version, int width, int height, int fps = DEFAULT_FRAMES_PER_SECOND);
    ~Game();

    virtual void update(float delta) {}
    virtual void draw(Graphics& g) {}

    void iterate();

    Graphics& getGraphics() { return graphics; }
    Audio& getAudio() { return audio; }
    Input& getInput() { return input; }
    ContentManager& getContent() { return content; }

    const std::string& getAppName() const { return appName; }
    const std::string& getAppIdentifier() const { return appIdentifier; }
    const std::string& getAppVersion() const { return appVersion; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    Graphics graphics;
    Audio audio;
    Input input;
    ContentManager content;

private:
    std::string appName;
    std::string appIdentifier;
    std::string appVersion;
    int width;
    int height;
    int framesPerSecond;
    int msPerFrame;
    uint64_t lastTime;
};

} // gdfx

#define CREATE_GAME(MyClass) gdfx::Game *gdfx::Game::createGame() { return new MyClass(); }

#endif // __GDFX_GAME_GAME_HPP__
