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
        input.addActionKey("left", Input::Key::LEFT);
        input.addActionKey("right", Input::Key::RIGHT);
        input.addActionKey("up", Input::Key::UP);
        input.addActionKey("down", Input::Key::DOWN);

        input.addActionAxis("left", Input::GamepadAxis::LEFTX, -1, 0.25);
        input.addActionAxis("right", Input::GamepadAxis::LEFTX, 1, 0.25);
        input.addActionAxis("up", Input::GamepadAxis::LEFTY, -1, 0.25);
        input.addActionAxis("down", Input::GamepadAxis::LEFTY, 1, 0.25);

        input.addActionButton("left", Input::GamepadButton::DPAD_LEFT);
        input.addActionButton("right", Input::GamepadButton::DPAD_RIGHT);
        input.addActionButton("up", Input::GamepadButton::DPAD_UP);
        input.addActionButton("down", Input::GamepadButton::DPAD_DOWN);

        input.addActionButton("reset", Input::GamepadButton::SOUTH);
        input.addActionKey("reset", Input::Key::SPACE);
    }

    void update(float delta)
    {
        if (input.getActionPressed("left"))
            x--;
        if (input.getActionPressed("right"))
            x++;

        if (input.getActionPressed("up"))
            y--;
        if (input.getActionPressed("down"))
            y++;

        if (input.getActionPressed("reset")) {
            x = getWidth() / 2;
            y = getHeight() / 2;
        }
    }

    void draw(Graphics& g)
    {
        g.setColor(Color::C64::YELLOW);
        g.clear();

        g.setColor(Color::C64::RED);
        g.drawFilledCircle(x, y, 20);

        g.setColor(Color::C64::WHITE);
        g.drawText("Move analog stick or press DPad or arrow keys to move.", 10, 10);
        g.drawText("Press SPACE or 'SOUTH' gamepad button to reset circle.", 10, 20);
    }

private:
    int x;
    int y;
};
CREATE_GAME(Ex006_HelloInput);
