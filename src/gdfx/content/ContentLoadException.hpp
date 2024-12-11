//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_PLATFORM_CONTENTLOADEXCEPTION_HPP__
#define __GDFX_PLATFORM_CONTENTLOADEXCEPTION_HPP__

#include <stdexcept>

namespace gdfx {

class ContentLoadException : public std::runtime_error {
public:
	ContentLoadException(const std::string& path) : runtime_error("Error loading content: " + path) {}
};

} // gdfx

#endif // __GDFX_PLATFORM_CONTENTLOADEXCEPTION_HPP__
