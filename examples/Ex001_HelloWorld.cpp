// Ex001_HelloWorld.cpp - A simple hello world example.

#include <gdfx/game/Game.hpp>
#include <gdfx/graphics/Graphics.hpp>

using namespace gdfx;

class Ex001_HelloWorld : public Game {
public:
    Ex001_HelloWorld() :
        Game("Example 001 - HelloWorld", "cc.gdfx.examples.helloworld", "1.0.0", 320, 180)
    {}

    void draw(Graphics& g)
    {
        g.setColor(0, 32, 192);
        g.clear();

        g.setColor(255, 255, 255);
        g.drawText("Hello World!", 0, 0);

        g.setColor(0, 0, 255);
        g.drawLine(10, 10, 100, 100);

        g.setColor(0, 255, 0);
        g.drawRect(20, 20, 50, 50);

        g.setColor(255, 255, 255);
        g.fillRect(80, 80, 20, 20);
    }
};
CREATE_GAME(Ex001_HelloWorld);
