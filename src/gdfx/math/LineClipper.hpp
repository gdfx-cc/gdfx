// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#ifndef GDFX_MATH_LINECLIPPER_HPP
#define GDFX_MATH_LINECLIPPER_HPP

#include <cstdint>

namespace gdfx {

/**
 * Cohen-Sutherland line clipping algorithm.
 */
class LineClipper {
public:
    /**
     * A bitmask of sides.
     */
    enum {
        TOP = 0x1,
        BOTTOM = 0x2,
        RIGHT = 0x4,
        LEFT = 0x8
    };

public:
    LineClipper();
    ~LineClipper();

    /**
     * Sets the current clipping rectangle dimensions.
     * @param x The starting x coordinate of the clipping rectangle.
     * @param y The starting y coordinate of the clipping rectangle.
     * @param w The width of the clipping rectangle.
     * @param h The height of the clipping rectangle.
     */
    void setClipRect(int x, int y, int w, int h);

    /**
     * Clips line endpoints to a clipping rectangle.
     * @param x0 First x coordinate.
     * @param y0 First y coordinate.
     * @param x1 Second x coordinate.
     * @param y1 Second y coordinate.
     * @return Returns true if the line is completely outside of the clip rectangle, else false.
     */
    bool clip(int& intX0, int& intY0, int& intX1, int& intY1) const;

    /**
     * Calculates a bitmask of all sides a point is outside of.
     * @param x The x coordinate of the point.
     * @param y The y coordinate of the point.
     * @return Returns a bitmask of sides the point is outside of.
     */
    uint32_t compOutCode(double x, double y) const;

private:
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};

} // gdfx

#endif // GDFX_MATH_LINECLIPPER_HPP
