//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/audio/Sound.hpp>
#include <gdfx/platform/SDLException.hpp>
#include <gdfx/content/ContentManager.hpp>

namespace gdfx {

Sound::Sound() :
    Content(),
    chunk(nullptr)
{
}

Sound::~Sound()
{
    unload();
}

bool Sound::load()
{
    unload();

    chunk = Mix_LoadWAV(getFullPath().c_str());
    if (!chunk)
        return false;
	
	return true;
}

void Sound::unload()
{
    if (chunk) {
        Mix_FreeChunk(chunk);
        chunk = nullptr;
    }
}

void Sound::play()
{
    if (!chunk)
        return;

    Mix_PlayChannel(-1, chunk, 1);
}

} // gdfx
