#ifndef PATCH_H
#define PATCH_H

#include <SFML/Graphics.hpp>

/**
 * A 9 patch is used to draw boxes with borders using 9 distinct segments with
 * the middle segments being stretched, but not the corner ones.
 */
class Patch {
    public:
        sf::IntRect bottomLeft;
        sf::IntRect left;
        sf::IntRect topLeft;
        sf::IntRect top;
        sf::IntRect topRight;
        sf::IntRect right;
        sf::IntRect bottomRight;
        sf::IntRect bottom;
        sf::IntRect middle;

        /**
         * Creates the patch and sets the inner rectangles correctly.
         * @param sprite is the main bounding rect.
         * @param left   is the number of pixels on the left to cut off.
         * @param top    is the number of pixels on the top to cut off.
         * @param right  is the number of pixels on the right to cut off.
         * @param bottom is the number of pixels on the buttom to cut off.
         */
        void fill(
            sf::IntRect sprite,
            int left,
            int top,
            int right,
            int bottom
        );
};

#endif
