//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/input/Input.hpp>

namespace gdfx {

Input::Input() :
	actions{},
	gamepad{ nullptr },
	keyboardState{ nullptr },
	axisState{},
	prevAxisState{},
	buttonState{},
	prevButtonState{}
{
}

Input::~Input()
{
	if (gamepad) {
		SDL_CloseGamepad(gamepad);
		gamepad = nullptr;
	}

	if (keyboardState) {
		delete[] keyboardState;
		keyboardState = nullptr;
	}
}

void Input::poll(uint32_t gameTimeElapsedMillis)
{
	pollKeyboard();
	pollController();
	pollActions(gameTimeElapsedMillis);
}

void Input::pollKeyboard()
{
	int numKeys;
	const bool *sdlKeyboardState = SDL_GetKeyboardState(&numKeys);
	if (sdlKeyboardState) {
		if (!keyboardState)
			keyboardState = new bool[numKeys];
		SDL_memcpy(keyboardState, sdlKeyboardState, numKeys * sizeof(bool));
	}
}

void Input::pollController()
{
	SDL_UpdateGamepads();

	if (!gamepad) {
		int numJoysticks;
		SDL_JoystickID *joysticks = SDL_GetJoysticks(&numJoysticks);
		for (int i = 0; i < numJoysticks; i++) {
			SDL_JoystickID joy = joysticks[i];
			if (SDL_IsGamepad(joy)) {
				gamepad = SDL_OpenGamepad(joy);
				if (gamepad) {
					char *mapping = SDL_GetGamepadMapping(gamepad);
					SDL_Log("Controller %i is mapped as \"%s\".", i, mapping);
					SDL_free(mapping);
					break;
				}
				else {
					// error opening game controller SDL_GetError()
				}
			}
		}
		SDL_free(joysticks);
	}

	if (gamepad) {
		for (int i = 0; i < SDL_GAMEPAD_AXIS_COUNT; i++)
			prevAxisState[i] = axisState[i];
		for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++)
			prevButtonState[i] = buttonState[i];

		if (SDL_GamepadConnected(gamepad)) {
			for (int i = 0; i < SDL_GAMEPAD_AXIS_COUNT; i++)
				axisState[i] = (float)SDL_GetGamepadAxis(gamepad, (SDL_GamepadAxis)i) / (float)SDL_JOYSTICK_AXIS_MAX;
			for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++)
				buttonState[i] = SDL_GetGamepadButton(gamepad, (SDL_GamepadButton)i);
		}
	}
}

void Input::pollActions(uint32_t gameTimeElapsedMillis)
{
	for (auto& it : actions)
		pollAction(it.second, gameTimeElapsedMillis);
}

void Input::pollAction(Action& action, uint32_t gameTimeElapsedMillis)
{
	float strength = 0.0f;
	bool state = false;

	if (action.axis != SDL_GAMEPAD_AXIS_INVALID) {
		if ((std::abs(axisState[action.axis]) > std::abs(action.axisDeadzone)) && (action.axisDirection * axisState[action.axis]) > 0.0f) {
			state = true;
			strength = std::abs(axisState[action.axis]); // TODO take deadzone into account
		}
	}

	if (action.button != SDL_GAMEPAD_BUTTON_INVALID && !state) {
		state = buttonState[action.button];
		strength = 1.0f;
	}

	if (action.scanCode != SDL_SCANCODE_UNKNOWN && !state) {
		state = keyboardState[action.scanCode];
		strength = 1.0f;
	}

	if (state) {
		if (action.state) {
			if (action.repeatEnabled) {
				action.nextPressInMillis -= gameTimeElapsedMillis;
				if (action.nextPressInMillis <= 0) {
					action.pressed = true;
					action.nextPressInMillis = action.repeatRateMillis;
				}
			}
		}
		else {
			action.state = true;
			action.strength = strength;
			action.pressed = true;
			if (action.repeatEnabled)
				action.nextPressInMillis = action.repeatDelayMillis;
		}
	}
	else {
		action.state = false;
		action.strength = 0.0f;
		action.pressed = false;
		if (action.repeatEnabled)
			action.nextPressInMillis = action.repeatDelayMillis;
	}
}

void Input::addActionAxis(const char *action, GamepadAxis axis, int direction, float deadzone)
{
	const std::string key(action);
	createAction(key);

	actions[key].axis = (SDL_GamepadAxis)axis;
	actions[key].axisDirection = direction;
	actions[key].axisDeadzone = deadzone;
}

void Input::addActionButton(const char *action, GamepadButton button)
{
	const std::string key(action);
	createAction(key);

	actions[key].button = (SDL_GamepadButton)button;
}

void Input::addActionKey(const char *action, Key scanCode)
{
	const std::string key(action);
	createAction(key);

	actions[key].scanCode = (SDL_Scancode)scanCode;
}

void Input::setActionRepeatEnabled(const char *action, bool enable)
{
	const std::string key(action);
	createAction(key);

	actions[key].repeatEnabled = enable;
}

void Input::setActionRepeatDelay(const char *action, int repeatDelayMillis, int repeatRateMillis)
{
	const std::string key(action);
	createAction(key);

	actions[key].repeatDelayMillis = repeatDelayMillis;
	actions[key].repeatRateMillis = repeatRateMillis;
	actions[key].nextPressInMillis = repeatDelayMillis;
}

bool Input::getActionState(const char *action)
{
	const std::string key(action);

	auto it = actions.find(key);
	if (it != actions.end())
		return actions[key].state;

	return false;
}

float Input::getActionStrength(const char *action)
{
	const std::string key(action);

	auto it = actions.find(key);
	if (it != actions.end())
		return actions[key].strength;

	return 0.0f;
}

bool Input::getActionPressed(const char *action)
{
	const std::string key(action);
	bool pressed = false;

	auto it = actions.find(key);
	if (it != actions.end()) {
		pressed = actions[key].pressed;
		actions[key].pressed = false;
	}

	return pressed;
}

bool Input::isKeyPressed(SDL_Scancode scanCode)
{
	return keyboardState[scanCode];
}

void Input::createAction(const std::string& key)
{
	auto it = actions.find(key);
	if (it == actions.end())
		actions.insert({ key, Action() });
}

} // gdfx