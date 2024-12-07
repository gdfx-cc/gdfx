// Ex001_HelloWorld.cpp - A simple hello world example.

#include <gdfx/game/Game.hpp>
#include <gdfx/graphics/Graphics.hpp>

using namespace gdfx;

class Ex001_HelloWorld : public Game {
public:
    Ex001_HelloWorld() :
        Game("Example 001 - HelloWorld", "cc.gdfx.examples.helloworld", "1.0.0", 160, 90)
    {}

    void draw(Graphics& g)
    {
        g.setColor(0, 64, 0);
        g.clear();

        g.setColor(255, 255, 255);
        g.drawText("Hello World!", 0, 0);
    }
};
CREATE_GAME(Ex001_HelloWorld);
