//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/platform/SDL3.hpp>
#include <gdfx/platform/SDLException.hpp>
#include <gdfx/audio/Audio.hpp>

namespace gdfx {

Audio::Audio() :
	initialized(false)
{
}

Audio::~Audio()
{
	destroy();
}

void Audio::create()
{
	destroy();

	if (Mix_OpenAudio(0, nullptr) < 0)
		throw SDLException();
	if (Mix_AllocateChannels(1) < 0)
		throw SDLException();

    initialized = true;
}

void Audio::destroy()
{
	if (!initialized)
        return;

    Mix_CloseAudio();

    initialized = false;
}

} // gdfx
