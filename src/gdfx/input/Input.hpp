//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_INPUT_INPUT_HPP__
#define __GDFX_INPUT_INPUT_HPP__

#include <stdint.h>
#include <unordered_map>
#include <string>

#include <gdfx/platform/SDL3.hpp>

namespace gdfx {

/**
* Input
*/
class Input {
public:
	enum class Key {
		UP = SDL_SCANCODE_UP,
		DOWN = SDL_SCANCODE_DOWN,
		LEFT = SDL_SCANCODE_LEFT,
		RIGHT = SDL_SCANCODE_RIGHT,
		SPACE = SDL_SCANCODE_SPACE
	};

	enum class GamepadAxis {
		LEFTX = SDL_GAMEPAD_AXIS_LEFTX,
		LEFTY = SDL_GAMEPAD_AXIS_LEFTY,
		RIGHTX = SDL_GAMEPAD_AXIS_RIGHTX,
		RIGHTY = SDL_GAMEPAD_AXIS_RIGHTY,
	};

	enum class GamepadButton {
		DPAD_UP = SDL_GAMEPAD_BUTTON_DPAD_UP,
		DPAD_DOWN = SDL_GAMEPAD_BUTTON_DPAD_DOWN,
		DPAD_LEFT = SDL_GAMEPAD_BUTTON_DPAD_LEFT,
		DPAD_RIGHT = SDL_GAMEPAD_BUTTON_DPAD_RIGHT,
		EAST = SDL_GAMEPAD_BUTTON_EAST,
		SOUTH = SDL_GAMEPAD_BUTTON_SOUTH,
		WEST = SDL_GAMEPAD_BUTTON_WEST,
		NORTH = SDL_GAMEPAD_BUTTON_NORTH,
		START = SDL_GAMEPAD_BUTTON_START,
		BACK = SDL_GAMEPAD_BUTTON_BACK
	};

	// only allows one of each axis/button/key per action (for now)
	struct Action {
		SDL_GamepadAxis axis;
		int axisDirection;
		float axisDeadzone;
		SDL_GamepadButton button;
		SDL_Scancode scanCode;
		bool pressed;
		bool state;
		float strength;
		bool repeatEnabled;
		int repeatDelayMillis;
		int repeatRateMillis;
		int nextPressInMillis;

		Action() :
			axis{ SDL_GAMEPAD_AXIS_INVALID },
			axisDirection{},
			axisDeadzone{},
			button{ SDL_GAMEPAD_BUTTON_INVALID },
			scanCode{ SDL_SCANCODE_UNKNOWN },
			pressed{},
			state{},
			strength{},
			repeatEnabled{true},
			repeatDelayMillis{500},
			repeatRateMillis{100},
			nextPressInMillis{}
		{
		}
	};
	typedef std::unordered_map<std::string, Action> ActionMap;

public:
	Input();
	~Input();

	void poll(uint32_t gameTimeElapsedMillis);
	void pollKeyboard();
	void pollController();
	void pollActions(uint32_t gameTimeElapsedMillis);
	void pollAction(Action& action, uint32_t gameTimeElapsedMillis);

	// setup
	void addActionAxis(const char *action, GamepadAxis axis, int direction, float deadzone);
	void addActionButton(const char *action, GamepadButton button);
	void addActionKey(const char *action, Key scanCode);

	// repeat config
	void setActionRepeatEnabled(const char *action, bool enable);
	void setActionRepeatDelay(const char *action, int repeatDelayMillis, int repeatRateMillis);

	// current state & strength of an action
	bool getActionState(const char *action);
	float getActionStrength(const char *action);

	// events w/optional repeat; clears 'pressed' status until next repeat (if enabled)
	bool getActionPressed(const char *action);

	bool isKeyPressed(SDL_Scancode scanCode);

private:
	void createAction(const std::string& key);

private:
	ActionMap actions;
	SDL_Gamepad *gamepad;
	bool *keyboardState;
	float axisState[SDL_GAMEPAD_AXIS_COUNT];
	float prevAxisState[SDL_GAMEPAD_AXIS_COUNT];
	bool buttonState[SDL_GAMEPAD_BUTTON_COUNT];
	bool prevButtonState[SDL_GAMEPAD_BUTTON_COUNT];
};

} // gdfx

#endif // __GDFX_INPUT_INPUT_HPP__
