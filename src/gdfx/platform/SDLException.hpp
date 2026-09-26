//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#ifndef GDFX_PLATFORM_SDLEXCEPTION_HPP
#define GDFX_PLATFORM_SDLEXCEPTION_HPP

#include <stdexcept>
#include <gdfx/platform/SDL3.hpp>

namespace gdfx {

class SDLException : public std::runtime_error {
public:
	SDLException() : runtime_error(SDL_GetError()) {}
};

} // gdfx

#endif // GDFX_PLATFORM_SDLEXCEPTION_HPP
