// SandBox Game - a simple game for miscellaneous experiments.

#include <gdfx/game/Game.hpp>
#include <gdfx/graphics/Graphics.hpp>

using namespace gdfx;

class SandBox : public Game {
public:
    SandBox() :
        Game("SandBox", "cc.gdfx.games.sandbox", "1.0.0", 640, 360)
    {
        font = content.load<Font>("data/font-8x8.png");
        blockTexture = content.load<Texture>("data/logo.png");
        x = y = 10;
        bx = getWidth() / 2;
        by = getHeight() / 2;
        xdir = -1;
        ydir = -1;
    }

    void update(float delta)
    {
        if (x++ > getWidth())
            x -= getWidth();
        if (y++ > getHeight())
            y -= getHeight();

        bx += xdir;
        by += ydir;

        if (xdir > 0) {
            if (bx > getWidth() - blockTexture->getWidth()) {
                xdir *= -1;
            }
        }
        else {
            if (bx < 0) {
                xdir *= -1;
            }
        }

        if (ydir > 0) {
            if (by > getHeight() - blockTexture->getHeight()) {
                ydir *= -1;
            }
        }
        else {
            if (by < 0) {
                ydir *= -1;
            }
        }
    }

    void draw(Graphics& g)
    {
        g.setColor(0, 32, 192);
        g.clear();

        g.drawTexture(*blockTexture, bx, by);

        g.setColor(Color::C64::WHITE);
        g.drawText(font.get(), x, y, "Hello World!");

        g.setColor(Color::C64::YELLOW);
        g.drawFilledRect(40, 40, 60, 60);
        g.drawTextRight(font.get(), getWidth(), 10, "GDFX GAME FRAMEWORK");

        g.setColor(Color::C64::CYAN);
        g.drawTextCentered(font.get(), getWidth()/2, getHeight()-font->getSize()*2, "(C) 2026 GDFX Authors");
    }

private:
    std::shared_ptr<Font> font;
    std::shared_ptr<Texture> blockTexture;
    int x, y;
    int bx, by;
    int xdir, ydir;
};
CREATE_GAME(SandBox);
