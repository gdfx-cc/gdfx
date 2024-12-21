// Ex007_HelloAnimation.cpp - A simple example using input.

#include <gdfx/game/Game.hpp>
#include <gdfx/graphics/Graphics.hpp>
#include <gdfx/graphics/Sprite.hpp>

using namespace gdfx;

class Ex007_HelloAnimation : public Game {
public:
    Ex007_HelloAnimation() :
        Game("Example 007 - HelloAnimation", "cc.gdfx.examples.helloanimation", "1.0.0", 32, 32)
    {
        image = content.load<Image>("data/animation.png");
        sprite.addFrames("drip", image, 0, 0, 32, 32, 5, 130, SpriteAnimation::Type::LOOP_FORWARD);
        sprite.playAnimation("drip");
    }

    void update(float delta)
    {
        sprite.update(delta);
    }

    void draw(Graphics& g)
    {
        g.setColor(0, 128, 64);
        g.clear();

        g.drawSprite(sprite, 0, 0);
    }

private:
    std::shared_ptr<Image> image;
    Sprite sprite;
};
CREATE_GAME(Ex007_HelloAnimation);
