//-----------------------------------------------------------------------------
// Copyright (C) 2026 GDFX Authors
//-----------------------------------------------------------------------------
#ifndef GDFX_PLATFORM_CONTENTLOADEXCEPTION_HPP
#define GDFX_PLATFORM_CONTENTLOADEXCEPTION_HPP

#include <stdexcept>

namespace gdfx {

class ContentLoadException : public std::runtime_error {
public:
	ContentLoadException(const std::string& path) : runtime_error("Error loading content: " + path) {}
};

} // gdfx

#endif // GDFX_PLATFORM_CONTENTLOADEXCEPTION_HPP
