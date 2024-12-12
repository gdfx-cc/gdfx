//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_AUDIO_SOUND_HPP__
#define __GDFX_AUDIO_SOUND_HPP__

#include <gdfx/platform/SDL3.hpp>
#include <gdfx/content/Content.hpp>

namespace gdfx {

class Sound : public Content {
protected:
    // protected ctor, use Sound::create() instead
    Sound();

public:
    virtual ~Sound();

    // for ContentManager
    static Sound *create() { return new Sound(); }

    virtual bool load() override;
    virtual void unload() override;

    void play();

private:
    Mix_Chunk *chunk;
};

} // gdfx

#endif // __GDFX_AUDIO_SOUND_HPP__
