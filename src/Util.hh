#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics.hpp>

/**
 * Contains random utility functions for your pleasure.
 * TODO: so much of this is loading stuff in, maybe I should have a different
 *       file just for loading related stuff.
 */
namespace Util {
    /**
     * Gives you a view that makes the screen letterbox so it scales stuff and
     * does blah blah blah you know.
     * @param view       is the view that was currently on.
     * @param dimensions is the new dimensions of the screen.
     * @return the new view to use.
     */
    sf::View getLetterboxView(sf::View view, sf::Vector2i dimensions);

    /**
     * Converts radians to degrees.
     * @param radians is the amount of radians to convert.
     * @return that amount in degrees.
     */
    float degrees(float radians);

    /**
     * Gives you the distance between two float vectors.
     * @param a is the first position.
     * @param b is the second position.
     * @return float the distance between these two positions.
     */
    float distance(sf::Vector2f a, sf::Vector2f b);

    /**
     * Gives you the manhattan distance between two float vectors. It is like
     * normal distance but shitty and fast.
     * @param a is the first position.
     * @param b is the second position.
     * @return the manhattan distance between the two positions.
     */
    float manhattan(sf::Vector2f a, sf::Vector2f b);
}

#endif
