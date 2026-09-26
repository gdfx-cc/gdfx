//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/audio/Sound.hpp>
#include <gdfx/platform/SDLException.hpp>
#include <gdfx/content/ContentManager.hpp>

namespace gdfx {

Sound::Sound() :
    Content(),
    audio(nullptr)
{
}

Sound::~Sound()
{
    unload();
}

bool Sound::load()
{
    unload();

    //audio = MIX_LoadAudio(device, getFullPath().c_str(), true);
    //if (!audio)
    //    return false;
	
	return true;
}

void Sound::unload()
{
    //if (audio) {
    //    MIX_DestroyAudio(audio);
    //    audio = nullptr;
    //}
}

void Sound::play()
{
    if (!audio)
        return;

    //Mix_PlayChannel(-1, audio, 1);
}

} // gdfx
