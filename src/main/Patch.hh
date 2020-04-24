#ifndef PATCH_H
#define PATCH_H

#include "Rectangle.hh"

/**
 * A 9 patch is used to draw boxes with borders using 9 distinct segments with
 * the middle segments being stretched, but not the corner ones.
 */
class Patch {
    public:
        Rectangle bottomLeft;
        Rectangle left;
        Rectangle topLeft;
        Rectangle top;
        Rectangle topRight;
        Rectangle right;
        Rectangle bottomRight;
        Rectangle bottom;
        Rectangle middle;

        /**
         * Creates the patch and sets the inner rectangles correctly.
         * @param sprite is the main bounding rect.
         * @param left   is the number of pixels on the left to cut off.
         * @param top    is the number of pixels on the top to cut off.
         * @param right  is the number of pixels on the right to cut off.
         * @param bottom is the number of pixels on the buttom to cut off.
         */
        void fill(
            Rectangle sprite,
            int left,
            int top,
            int right,
            int bottom
        );
};

#endif
