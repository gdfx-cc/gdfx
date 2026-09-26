//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/platform/SDL3.hpp>
#include <gdfx/platform/SDLException.hpp>
#include <gdfx/audio/Audio.hpp>

namespace gdfx {

Audio::Audio() :
	device(nullptr),
	track(nullptr),
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

	if (!MIX_Init())
		throw SDLException();

	device = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	if (!device)
		throw SDLException();

	track = MIX_CreateTrack(device);
	if (!track)
		throw SDLException();

    initialized = true;
}

void Audio::destroy()
{
	if (!initialized)
        return;

	MIX_DestroyTrack(track);
	track = nullptr;

	MIX_DestroyMixer(device);
	device = nullptr;

    MIX_Quit();
    initialized = false;
}

} // gdfx
