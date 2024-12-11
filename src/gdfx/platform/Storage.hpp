//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_PLATFORM_STORAGE_HPP__
#define __GDFX_PLATFORM_STORAGE_HPP__

#include <cstdint>

namespace gdfx {

class Storage {
public:
    Storage();
    ~Storage();

    static char *   loadFile(const char *file);
    static void     unloadFile(char*& data);
    static uint8_t *loadImage(const char *file, int& w, int& h, int& bytesPerPixel, int desiredBytesPerPixel, bool flipY = true);
    static void     unloadImage(uint8_t*& data);
};

} // gdfx

#endif // __GDFX_PLATFORM_STORAGE_HPP__
