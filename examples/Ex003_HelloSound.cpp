// Ex003_HelloSound.cpp - Loads and plays a sound.

#include <memory>
#include <gdfx/game/Game.hpp>
#include <gdfx/graphics/Graphics.hpp>
#include <gdfx/audio/Sound.hpp>

using namespace gdfx;

class Ex003_HelloSound : public Game {
public:
    Ex003_HelloSound() :
        Game("Example 003 - HelloSound", "cc.gdfx.examples.hellosound", "1.0.0", 320, 180)
    {
        sound = content.load<Sound>("data/sfx.mp3");
        sound->play();
    }

    void draw(Graphics& g)
    {
        g.setColor(Color::C64::RED);
        g.clear();
    }

private:
    std::shared_ptr<Sound> sound;
};
CREATE_GAME(Ex003_HelloSound);
