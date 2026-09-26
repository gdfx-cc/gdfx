// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#ifndef GDFX_GRAPHICS_CANVAS_IMAGE_HPP
#define GDFX_GRAPHICS_CANVAS_IMAGE_HPP

#include <cstdint>
#include <gdfx/graphics/canvas/Canvas.hpp>
#include <gdfx/graphics/canvas/Palette.hpp>
#include <gdfx/content/Content.hpp>

namespace gdfx {

/**
 * Indexed software image.
 */
class Image : public Content, public Canvas {
protected:
    // protected ctor, use Image::create() instead
    Image();

public:
    virtual ~Image();

    // for ContentManager
    static Image *create() { return new Image(); }

    virtual bool load() override;
    virtual void unload() override;

    void save(const char *path);
};

} // gdfx

#endif // GDFX_GRAPHICS_CANVAS_IMAGE_HPP

