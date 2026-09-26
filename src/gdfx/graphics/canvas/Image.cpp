// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#include <cstdio>
#include <gdfx/graphics/canvas/Image.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <gdfx/platform/stb_image_write.h>

#if defined(_MSC_VER)
    #define PACKED_STRUCT_BEGIN __pragma(pack(push, 1))
    #define PACKED_STRUCT_END   __pragma(pack(pop))
    #define PACKED
#elif defined(__GNUC__) || defined(__clang__)
    #define PACKED_STRUCT_BEGIN
    #define PACKED_STRUCT_END
    #define PACKED __attribute__((packed))
#else
    #error "Unknown compiler. Please define packing macros for this compiler."
#endif

namespace gdfx {

PACKED_STRUCT_BEGIN
struct PACKED PCXHeader {
    uint8_t magic; // always 10
    uint8_t version;
    uint8_t encoding; // always 1, ie RLE
    uint8_t bitsPerPixel; // 8
    uint16_t minX;
    uint16_t minY;
    uint16_t maxX;
    uint16_t maxY;
    uint16_t horizontalDPI;
    uint16_t verticalDPI;
    uint8_t egaPalette[48];
    uint8_t reserved;
    uint8_t numColorPlanes;
    uint16_t numBytesPerLine;
    uint16_t paletteType;
    uint16_t horizontalResolution;
    uint16_t verticalResolution;
    uint8_t reservedExt[54];
};
PACKED_STRUCT_END

Image::Image() :
    Content(),
    Canvas()
{
}

Image::~Image()
{
    destroy();
}

bool Image::load()
{
    destroy();

    FILE *fp = std::fopen(getFullPath().c_str(), "rb");
    if (!fp)
        return false;

    PCXHeader header;
    if (std::fread(&header, sizeof(header), 1, fp) != 1) {
        std::fclose(fp);
        return false;
    }

    if (header.magic != 10 || header.encoding != 1 || header.bitsPerPixel != 8) {
        std::fclose(fp);
        return false;
    }

    int w = header.maxX - header.minX + 1;
    int h = header.maxY - header.minY + 1;

    Canvas::create(w, h);

    const int size = w * h;
    int count = 0;
    int numBytes = 0;
    uint8_t data = 0;
    uint8_t *buffer = getBuffer();

    while (count < size) {
        data = (uint8_t)fgetc(fp);
        const bool rle = data >= 192 && data <= 255;

        if (rle) {
            numBytes = data - 192;
            data = (uint8_t)fgetc(fp); // get actual data

            while (numBytes-- > 0 && count < size) {
                buffer[count++] = data;
            }
        }
        else {
            // actual data
            buffer[count++] = data;
        }
    }

    // rewind from end of file to get palette
    std::fseek(fp, -768L, SEEK_END);

    // load palette
    for (int i = 0; i < 256; i++) {
        const uint8_t r = (uint8_t)fgetc(fp);
        const uint8_t g = (uint8_t)fgetc(fp);
        const uint8_t b = (uint8_t)fgetc(fp);

        getPalette().setColor(i, Color(r, g, b));
    }

    std::fclose(fp);
    return true;
}

void Image::unload()
{
    destroy();
}

void Image::save(const char *path)
{
    // TODO: switch to indexed image writer.

    const int size = getWidth() * getHeight();
    // need to convert 8-bit to 24-bit format
    uint8_t *data = new uint8_t[size * 3];

    for (int i = 0; i <size; i++) {
        data[i * 3 + 0] = getPalette().getColor(getBuffer()[i]).r;
        data[i * 3 + 1] = getPalette().getColor(getBuffer()[i]).g;
        data[i * 3 + 2] = getPalette().getColor(getBuffer()[i]).b;
    }

    stbi_write_png(path, getWidth(), getHeight(), 3, data, getWidth() * 3);
    delete[] data;
}

} // gdfx

