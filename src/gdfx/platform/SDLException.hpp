//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_PLATFORM_SDLEXCEPTION_HPP__
#define __GDFX_PLATFORM_SDLEXCEPTION_HPP__

#include <stdexcept>
#include <gdfx/platform/SDL3.hpp>

namespace gdfx {

class SDLException : public std::runtime_error {
public:
	SDLException() : runtime_error(SDL_GetError()) {}
};

} // gdfx

#endif // __GDFX_PLATFORM_SDLEXCEPTION_HPP__
