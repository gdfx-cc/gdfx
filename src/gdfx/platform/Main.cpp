//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <iostream>
#include <gdfx/platform/SDL3.hpp>
#include <gdfx/game/Game.hpp>

#define SDL_MAIN_USE_CALLBACKS

using namespace gdfx;

SDL_AppResult SDL_AppInit(void **state, int argc, char **argv)
{
	Game *game = nullptr;
	*state = nullptr;

	try {
		game = Game::createGame();
		*state = game;
	}
	catch (const std::exception& e) {
		std::string title(game ? game->getAppName(): "GDFX Game");
		if (!SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), e.what(), nullptr)) {
			std::cerr << title << ": " << e.what() << std::endl;
		}
		return SDL_APP_FAILURE;
	}
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *state)
{
	Game *game = (Game *)state;

	try {
		if (game) {
			game->update();
			game->draw();
		}
	}
	catch (const std::exception& e) {
		std::string title(game ? game->getAppName(): "GDFX Game");
		if (!SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), e.what(), nullptr)) {
			std::cerr << title << ": " << e.what() << std::endl;
		}
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE; // or SDL_APP_FAILURE to exit with failure or SDL_APP_SUCCESS to exit with success
}

SDL_AppResult SDL_AppEvent(void *state, SDL_Event *event)
{
	Game *game = (Game *)state;

	if (event->type == SDL_EVENT_QUIT)
		return SDL_APP_SUCCESS;

	try {
		if (game) {
			// TODO: handle game events
		}
	}
	catch (const std::exception& e) {
		std::string title(game ? game->getAppName(): "GDFX Game");
		if (!SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), e.what(), nullptr)) {
			std::cerr << title << ": " << e.what() << std::endl;
		}
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE; // same as AppIterate
}

void SDL_AppQuit(void *state, SDL_AppResult result)
{
	Game *game = (Game *)state;

	try {
		if (game) {
			delete game;
			game = nullptr;
		}
	}
	catch (const std::exception& e) {
		std::string title(game ? game->getAppName(): "GDFX Game");
		if (!SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), e.what(), nullptr)) {
			std::cerr << title << ": " << e.what() << std::endl;
		}
	}
}
