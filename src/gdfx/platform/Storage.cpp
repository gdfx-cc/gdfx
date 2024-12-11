//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#include <cstdio>
#include <gdfx/platform/Storage.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <gdfx/platform/stb_image.h>

namespace gdfx {

Storage::Storage()
{
}

Storage::~Storage()
{
}

/**
* loadFile
* Loads file contents and returns a new pointer to the memory.
* The caller is responsible for freeing the pointer (delete[] ptr).
*/
char *Storage::loadFile(const char *file)
{
    FILE *fp = std::fopen(file, "rb");
    if (!fp)
        return nullptr;

    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);

    char *buffer = new char[fileSize+1];
    rewind(fp);
    fread(buffer, sizeof(char), fileSize, fp);
    buffer[fileSize] = '\0';

    return buffer;
}

/**
* unloadFile
* Frees the memory previous allocated by loadFile().
*/
void Storage::unloadFile(char*& data)
{
    if (data) {
        delete[] data;
        data = nullptr;
    }
}

/**
* loadImage
* Loads an image from disk, returning the byte array.
* 
* @param file The file to load
* @param w The width of the file that was loaded
* @param h The height of the file that was loaded
* @param bytesPerPixel The bytes per pixel format of the returned pointer
* @param desiredBytesPerPixel How many bytes per pixel are desired (converts as necessary, if possible)
*/
uint8_t *Storage::loadImage(const char *file, int& w, int& h, int& bytesPerPixel, int desiredBytesPerPixel, bool flipY)
{
    stbi_set_flip_vertically_on_load(flipY);
    return stbi_load(file, &w, &h, &bytesPerPixel, desiredBytesPerPixel);
}

/**
* unloadImage
* Frees the pointer previously returned by loadImage
*/
void Storage::unloadImage(uint8_t*& data)
{
    if (data) {
        stbi_image_free(data);
        data = nullptr;
    }
}

} // gdfx
