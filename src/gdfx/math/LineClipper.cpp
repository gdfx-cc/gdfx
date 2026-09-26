// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#include <gdfx/math/LineClipper.hpp>

namespace gdfx {

LineClipper::LineClipper() :
    xmin(0),
    xmax(0),
    ymin(0),
    ymax(0)
{
}

LineClipper::~LineClipper()
{
}

void LineClipper::setClipRect(int x, int y, int w, int h)
{
    xmin = x;
    xmax = x + w - 1;
    ymin = y;
    ymax = y + h - 1;
}

/**
 * Cohen-Sutherland algorithm for clipping lines from "Computer Graphics: Principles and Practice".
 * @param intX0
 * @param intY0
 * @param intX1
 * @param intY1
 * @return Returns true if the entire line has been clipped and is not visible, else false (partial or full line).
 */
bool LineClipper::clip(int& intX0, int& intY0, int& intX1, int& intY1) const
{
    double x0 = intX0;
    double y0 = intY0;
    double x1 = intX1;
    double y1 = intY1;

    uint32_t outcode0, outcode1, outcodeOut;
    bool accept = false;
    bool done = false;

    outcode0 = compOutCode(x0, y0);
    outcode1 = compOutCode(x1, y1);

    do {
        if (!(outcode0 | outcode1)) {
            // trivial accept and exit for lines completely inside clip rect
            accept = true;
            done = true;
        }
        else if (outcode0 & outcode1) {
            // logical and is true, so trivial reject and exit
            done = true;
        }
        else {
            // failed both tests, so calculate the lnie segment to clip
            // from an outside point to an intersection with clip edge
            double x, y;
            // at least one endpoint is outside the clip rectangle: pick it.
            outcodeOut = outcode0 ? outcode0 : outcode1;
            // now find intersection point
            if (outcodeOut & TOP) { // divide line at top of clip rect
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            }
            else if (outcodeOut & BOTTOM) { // divide line at bottom edge of clip rect
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            }
            else if (outcodeOut & RIGHT) { // divide line at right edge of clip rect
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            }
            else { // divide line at left edge of lip rect
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            // now move outside point to intersection point to clip and get ready for next pass
            if (outcodeOut == outcode0) {
                x0 = x;
                y0 = y;
                outcode0 = compOutCode(x0, y0);
            }
            else {
                x1 = x;
                y1 = y;
                outcode1 = compOutCode(x1, y1);
            }
        } // sub-divide
    } while (!done);

    if (accept) {
        intX0 = (int)x0;
        intY0 = (int)y0;
        intX1 = (int)x1;
        intY1 = (int)y1;
        return false; // false means the whole line was not clipped and a partial line should be drawn
    }

    return true; // entire line has been clipped and is invisible and should not be drawn
}

uint32_t LineClipper::compOutCode(double x, double y) const
{
    uint32_t code = 0;
    if (y > ymax) {
        code |= TOP;
    }
    else if (y < ymin) {
        code |= BOTTOM;
    }
    if (x > xmax) {
        code |= RIGHT;
    }
    else if (x < xmin) {
        code |= LEFT;
    }
    return code;
}

} // gdfx

