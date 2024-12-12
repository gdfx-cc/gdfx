//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_AUDIO_AUDIO_HPP__
#define __GDFX_AUDIO_AUDIO_HPP__

namespace gdfx {

class Audio {
public:
	Audio();
	~Audio();

	void create();
	void destroy();

private:
	bool initialized;
};

} // gdfx

#endif // __GDFX_AUDIO_AUDIO_HPP__
