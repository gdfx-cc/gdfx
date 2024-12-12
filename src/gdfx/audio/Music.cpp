//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/audio/Music.hpp>
#include <gdfx/platform/SDLException.hpp>
#include <gdfx/content/ContentManager.hpp>

namespace gdfx {

Music::Music() :
    Content(),
    song(nullptr)
{
}

Music::~Music()
{
    unload();
}

bool Music::load()
{
    unload();

    song = Mix_LoadMUS(getFullPath().c_str());
    if (!song)
        return false;
	
	return true;
}

void Music::unload()
{
    if (song) {
        Mix_FreeMusic(song);
        song = nullptr;
    }
}

void Music::play()
{
    if (!song)
        return;

    Mix_PlayMusic(song, -1);
}

void Music::pause()
{
    Mix_PauseMusic();
}

void Music::resume()
{
    Mix_ResumeMusic();
}

} // gdfx
