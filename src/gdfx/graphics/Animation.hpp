//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_GRAPHICS_ANIMATION_HPP__
#define __GDFX_GRAPHICS_ANIMATION_HPP__

#include <vector>

namespace gdfx {

/**
* Animation
*/
template <typename T>
class Animation {
public:
	enum class Type {
		NONE,
		FORWARD,
		REVERSE,
		LOOP_FORWARD,
		LOOP_REVERSE,
		PING_PONG
	};

	enum class State {
		STOPPED,
		PLAYING,
		PAUSED
	};

	struct Frame {
		T data;
		int delayMillis;
		Frame(T data, int delayMillis) : data(data), delayMillis(delayMillis) {}
	};

public:
	Animation() : frames{}, frameIndex{}, framePing{ true }, type{}, state{ State::STOPPED }, accumMillis{} {}
	~Animation() {}

	void update(float delta)
	{
		if (getFrameCount() == 0)
			return;
		if (getType() == Type::NONE || getState() != State::PLAYING)
			return;

		if (getFrame().delayMillis > 0) {
			int elapsedMillis = (int)(delta * 1000.0f);
			accumMillis += elapsedMillis;

			while (accumMillis >= getFrame().delayMillis) {
				accumMillis -= getFrame().delayMillis;
				advanceFrame();
			}
		}
		else
			advanceFrame();
	}

	Type getType() const { return type; }
	void setType(Type type)
	{
		this->type = type;
		state = State::STOPPED;
		frameIndex = 0;
		framePing = true;
	}

	State getState() const { return state; }
	void setState(State state) { this->state = state; }

	void play() // always starts at first frame
	{
		if (getType() == Type::NONE)
			return;
		frameIndex = 0;
		framePing = true;
		state = State::PLAYING;
		accumMillis = 0;
	}
	void stop()
	{
		frameIndex = 0;
		framePing = true;
		state = State::STOPPED;
		accumMillis = 0;
	}
	void pause()
	{
		if (getType() == Type::NONE)
			return;
		state = State::PAUSED;
		accumMillis = 0;
	}
	void resume()
	{
		if (getType() == Type::NONE)
			return;
		state = State::PLAYING;
		accumMillis = 0;
	}

	void addFrame(T data, int delayMillis)
	{
		frames.emplace_back(data, delayMillis);
	}

	Frame& getFrame() { return frames[frameIndex]; }
	T& getFrameData() { return frames[frameIndex].data; }
	int getFrameDelayMillis() { return frames[frameIndex].delayMillis; }
	int getFrameCount() const { return (int)frames.size(); }
	int getFrameIndex() const { return frameIndex; }
	void setFrameIndex(int index)
	{
		if (index >= 0 && index < getFrameCount())
			frameIndex = index;
	}
	
	void advanceFrame()
	{
		if (getFrameCount() <= 1)
			return;

		const int endIndex = getFrameCount() - 1;

		switch (type) {
		case Type::NONE:
			break;
		case Type::FORWARD:
			if (frameIndex < endIndex)
				frameIndex++;
			else {
				state = State::STOPPED;
				accumMillis = 0;
			}
			break;
		case Type::REVERSE:
			if (frameIndex > 0)
				frameIndex--;
			else {
				state = State::STOPPED;
				accumMillis = 0;
			}
			break;
		case Type::LOOP_FORWARD:
			if (++frameIndex > endIndex)
				frameIndex = 0;
			break;
		case Type::LOOP_REVERSE:
			if (--frameIndex < 0)
				frameIndex = endIndex;
			break;
		case Type::PING_PONG:
			if (framePing) {
				if (frameIndex < endIndex)
					frameIndex++;
				else {
					frameIndex--;
					framePing = false;
				}
			}
			else { // pong
				if (frameIndex > 0)
					frameIndex--;
				else {
					frameIndex++;
					framePing = true;
				}
			}
			break;
		default:
			break;
		}
	}

private:
	std::vector<Frame> frames;
	int frameIndex;
	bool framePing;
	Type type;
	State state;
	int accumMillis;
};

} // gdfx

#endif // __GDFX_GRAPHICS_ANIMATION_HPP__
