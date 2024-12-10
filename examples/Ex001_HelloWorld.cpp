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
        g.drawFilledRect(80, 80, 20, 20);

        g.setColor(255, 0, 0);
        g.drawTriangle(65, 80, 120, 90, 100, 170);
        g.drawFilledTriangle(125, 90, 180, 100, 160, 175);

        g.setColor(0, 255, 0);
        g.drawCircle(100, 100, 55);

        g.setColor(0, 0, 128);
        g.drawFilledCircle(200, 100, 32);
    }
};
CREATE_GAME(Ex001_HelloWorld);
