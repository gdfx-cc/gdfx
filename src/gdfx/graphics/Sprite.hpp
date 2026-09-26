//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#ifndef GDFX_GRAPHICS_SPRITE_HPP
#define GDFX_GRAPHICS_SPRITE_HPP

#include <string>
#include <unordered_map>
#include <memory>

#include <gdfx/graphics/Animation.hpp>
#include <gdfx/graphics/TextureRegion.hpp>

namespace gdfx {

typedef Animation<TextureRegion> SpriteAnimation;

/**
* Sprite
*/
class Sprite {
public:
	Sprite();
	virtual ~Sprite();

	void update(float delta);

	void addFrames(const std::string& name, std::shared_ptr<Texture> texture, int x, int y, int w, int h, int count, int delayMillis, SpriteAnimation::Type type = SpriteAnimation::Type::FORWARD);
	void addAnimation(const std::string& name, SpriteAnimation& anim);

	TextureRegion& getFrameTextureRegion()
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

#endif // GDFX_GRAPHICS_SPRITE_HPP
