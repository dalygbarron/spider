#ifndef HELP_H
#define HELP_H

#include <SFML/Graphics.hpp>

/**
 * Contains helper functions for tests.
 */
namespace Help {
    /**
     * Asserts that two numbers are close in value.
     * @param a is the first number.
     * @param b is the second number.
     */
    void close(float a, float b);

    /**
     * Compares two 2d vectors.
     * @param a is the first vector.
     * @param b is the second vector.
     */
    void compareVector2f(sf::Vector2f a, sf::Vector2f b);

    /**
     * Compares two 2D vectors as if they were latitudes and longitudes on
     * a sphere, and in such a way that PI is approximately equal to 0.
     * @param a is the first point.
     * @param b is the second point.
     */
    void compareSphere(sf::Vector2f a, sf::Vector2f b);
};

#endif
