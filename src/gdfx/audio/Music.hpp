//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_AUDIO_MUSIC_HPP__
#define __GDFX_AUDIO_MUSIC_HPP__

#include <gdfx/platform/SDL3.hpp>
#include <gdfx/content/Content.hpp>

namespace gdfx {

class Music : public Content {
protected:
    // protected ctor, use Music::create() instead
    Music();

public:
    virtual ~Music();

    // for ContentManager
    static Music *create() { return new Music(); }

    virtual bool load() override;
    virtual void unload() override;

    void play();
    void pause();
    void resume();

private:
    Mix_Music *song;
};

} // gdfx

#endif // __GDFX_AUDIO_MUSIC_HPP__
