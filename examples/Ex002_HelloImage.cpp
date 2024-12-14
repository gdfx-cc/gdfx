// Ex002_HelloImage.cpp - Loads and displays an image on the screen.

#include <memory>
#include <gdfx/game/Game.hpp>
#include <gdfx/graphics/Graphics.hpp>
#include <gdfx/graphics/Image.hpp>

using namespace gdfx;

class Ex002_HelloImage : public Game {
public:
    Ex002_HelloImage() :
        Game("Example 002 - HelloImage", "cc.gdfx.examples.helloimage", "1.0.0", 320, 180)
    {
        image = content.load<Image>("data/image.png");
    }

    void draw(Graphics& g)
    {
        g.clear();
        g.drawImage(*image, getWidth()/2 - image->getWidth()/2, getHeight()/2 - image->getHeight()/2);
    }

private:
    std::shared_ptr<Image> image;
};
CREATE_GAME(Ex002_HelloImage);
