//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#ifndef GDFX_AUDIO_SOUND_HPP
#define GDFX_AUDIO_SOUND_HPP

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
    MIX_Audio *audio;
};

} // gdfx

#endif // GDFX_AUDIO_SOUND_HPP
