//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_GRAPHICS_SPRITE_HPP__
#define __GDFX_GRAPHICS_SPRITE_HPP__

#include <string>
#include <unordered_map>
#include <memory>

#include <gdfx/graphics/Graphics.hpp>
#include <gdfx/graphics/Animation.hpp>
#include <gdfx/graphics/ImageRegion.hpp>

namespace gdfx {

typedef Animation<ImageRegion> SpriteAnimation;

/**
* Sprite
*/
class Sprite {
public:
	Sprite();
	virtual ~Sprite();

	void update(float delta);
	void draw(Graphics& g, int x, int y);

	void addFrames(const std::string& name, std::shared_ptr<Image> image, int x, int y, int w, int h, int count, int delayMillis, SpriteAnimation::Type type = SpriteAnimation::Type::FORWARD);
	void addAnimation(const std::string& name, SpriteAnimation& anim);

	ImageRegion& getFrameImageRegion()
	{
		auto it = animations.find(currentAnimation);
		return it->second.getFrameData();
	}
	
	void setFrameIndex(const std::string& animationName, int frameIndex);
	void playAnimation(const std::string& name);
	void stopAnimation();
	void pauseAnimation();
	void resumeAnimation();
	bool isAnimationPlaying();
	
private:
	std::unordered_map<std::string, SpriteAnimation> animations;
	std::string currentAnimation;
};

} // gdfx

#endif // __GDFX_GRAPHICS_SPRITE_HPP__
