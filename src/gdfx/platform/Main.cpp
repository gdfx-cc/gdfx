//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/platform/SDL3.hpp>
#include <gdfx/game/Game.hpp>

#define SDL_MAIN_USE_CALLBACKS

using namespace gdfx;

SDL_AppResult SDL_AppInit(void **state, int argc, char **argv)
{
	Game *game = Game::createGame();
	*state = game;
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *state)
{
	// TODO game update/draw
	return SDL_APP_CONTINUE; // or SDL_APP_FAILURE to exit with failure or SDL_APP_SUCCESS to exit with success
}

SDL_AppResult SDL_AppEvent(void *state, SDL_Event *event)
{
	if (event->type == SDL_EVENT_QUIT)
		return SDL_APP_SUCCESS;

	// TODO: handle game events

	return SDL_APP_CONTINUE; // same as AppIterate
}

void SDL_AppQuit(void *state, SDL_AppResult result)
{
	Game *game = (Game *)state;
	delete game;
	game = nullptr;
}
