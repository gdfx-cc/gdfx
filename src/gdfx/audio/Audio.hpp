//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#ifndef GDFX_AUDIO_AUDIO_HPP
#define GDFX_AUDIO_AUDIO_HPP

#include <gdfx/platform/SDL3.hpp>

namespace gdfx {

class Audio {
public:
	Audio();
	~Audio();

	void create();
	void destroy();

private:
	MIX_Mixer *device;
	MIX_Track *track;
	bool initialized;
};

} // gdfx

#endif // GDFX_AUDIO_AUDIO_HPP
