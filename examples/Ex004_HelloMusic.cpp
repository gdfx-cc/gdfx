// Ex004_HelloMusic.cpp - Loads and plays music.

#include <memory>
#include <gdfx/game/Game.hpp>
#include <gdfx/graphics/Graphics.hpp>
#include <gdfx/audio/Music.hpp>

using namespace gdfx;

class Ex004_HelloMusic : public Game {
public:
    Ex004_HelloMusic() :
        Game("Example 004 - HelloMusic", "cc.gdfx.examples.hellomusic", "1.0.0", 320, 180)
    {
        music = content.load<Music>("data/song.mp3");
        music->play();
    }

    void draw(Graphics& g)
    {
        g.setColor(0, 255, 0);
        g.clear();
    }

private:
    std::shared_ptr<Music> music;
};
CREATE_GAME(Ex004_HelloMusic);
