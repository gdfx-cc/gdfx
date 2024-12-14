// Ex006_HelloInput.cpp - A simple example using input.

#include <gdfx/game/Game.hpp>
#include <gdfx/graphics/Graphics.hpp>

using namespace gdfx;

class Ex006_HelloInput : public Game {
public:
    Ex006_HelloInput() :
        Game("Example 006 - HelloInput", "cc.gdfx.examples.helloinput", "1.0.0", 640, 360),
        x(getWidth() / 2),
        y(getHeight() / 2)
    {
        getInput().addActionKey("left", Input::Key::LEFT);
        getInput().addActionKey("right", Input::Key::RIGHT);
        getInput().addActionKey("up", Input::Key::UP);
        getInput().addActionKey("down", Input::Key::DOWN);

        getInput().addActionAxis("left", Input::GamepadAxis::LEFTX, -1, 0.25);
        getInput().addActionAxis("right", Input::GamepadAxis::LEFTX, 1, 0.25);
        getInput().addActionAxis("up", Input::GamepadAxis::LEFTY, -1, 0.25);
        getInput().addActionAxis("down", Input::GamepadAxis::LEFTY, 1, 0.25);

        getInput().addActionButton("left", Input::GamepadButton::DPAD_LEFT);
        getInput().addActionButton("right", Input::GamepadButton::DPAD_RIGHT);
        getInput().addActionButton("up", Input::GamepadButton::DPAD_UP);
        getInput().addActionButton("down", Input::GamepadButton::DPAD_DOWN);

        getInput().addActionButton("reset", Input::GamepadButton::SOUTH);
        getInput().addActionKey("reset", Input::Key::SPACE);
    }

    void update()
    {
        if (getInput().getActionPressed("left"))
            x--;
        if (getInput().getActionPressed("right"))
            x++;

        if (getInput().getActionPressed("up"))
            y--;
        if (getInput().getActionPressed("down"))
            y++;

        if (getInput().getActionPressed("reset")) {
            x = getWidth() / 2;
            y = getHeight() / 2;
        }
    }

    void draw(Graphics& g)
    {
        g.setColor(0, 64, 64);
        g.clear();

        g.setColor(255, 0, 0);
        g.drawFilledCircle(x, y, 20);

        g.setColor(255, 255, 255);
        g.drawText("Move analog stick or press DPad or arrow keys to move.", 10, 10);
        g.drawText("Press SPACE or 'SOUTH' gamepad button to reset circle.", 10, 20);
    }

private:
    int x;
    int y;
};
CREATE_GAME(Ex006_HelloInput);