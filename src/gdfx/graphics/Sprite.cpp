//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------

#include <gdfx/graphics/Sprite.hpp>

namespace gdfx {

Sprite::Sprite() :
	animations{},
	currentAnimation{}
{
}

Sprite::~Sprite()
{
}

void Sprite::update(float delta)
{
	if (animations.size() > 0 && currentAnimation.length() > 0)
		animations[currentAnimation].update(delta);
}

void Sprite::addFrames(const std::string& name, std::shared_ptr<Image> image, int x, int y, int w, int h, int count, int delayMillis, SpriteAnimation::Type type)
{
	SpriteAnimation anim;

	anim.setType(type);
	
	int sx = x;
	int sy = y;
	for (int i = 0; i < count; i++) {
		anim.addFrame(ImageRegion(image, sx, sy, w, h), delayMillis);
		sx += w;
		if (sx >= image->getWidth()) {
			sx = 0;
			sy += h;
		}
	}
	addAnimation(name, anim);

	if (currentAnimation.length() == 0)
		currentAnimation = name;
}

void Sprite::addAnimation(const std::string& name, SpriteAnimation& anim)
{
	animations.insert({ name, anim });
}

void Sprite::setFrameIndex(const std::string& animationName, int frameIndex)
{
	currentAnimation = animationName;
	animations[currentAnimation].setFrameIndex(frameIndex);
}

void Sprite::playAnimation(const std::string& name)
{
	currentAnimation = name;
	animations[currentAnimation].play();
}

void Sprite::stopAnimation()
{
	animations[currentAnimation].stop();
}

void Sprite::pauseAnimation()
{
	animations[currentAnimation].pause();
}

void Sprite::resumeAnimation()
{
	animations[currentAnimation].resume();
}

bool Sprite::isAnimationPlaying()
{
	return animations[currentAnimation].getState() == SpriteAnimation::State::PLAYING;
}

} // gdfx