//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <gdfx/platform/SDL3.hpp>
#include <gdfx/content/Content.hpp>

namespace gdfx {

void Content::setPath(const std::string& path)
{
    this->path = path;

    char *fullPathStr = nullptr;
    SDL_asprintf(&fullPathStr, "%s%s", SDL_GetBasePath(), path.c_str());
    fullPath = fullPathStr;
    SDL_free(fullPathStr);
}

} // gdfx
