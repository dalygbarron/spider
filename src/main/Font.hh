#ifndef FONT_H
#define FONT_H

#include <SFML/Graphics.hpp>

/**
 * Represents a font that can draw letters and all that.
 */
class Font {
    public:
        sf::IntRect bounds;
        sf::Vector2i character;

        /**
         * Default constructor for making junk fonts to take up space.
         */
        Font();

        /**
         * Creates the font by setting it's bounds.
         * @param bounds are the bounds withing which all the characters of the
         *               font lie.
         */
        Font(sf::IntRect bounds);

        /**
         * Gives you the bounds for a given character drawn with this font.
         */
        sf::IntRect get(char c);
};

#endif
