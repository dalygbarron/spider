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
     * Compares two 2d vectors.
     * @param a is the first vector.
     * @param b is the second vector.
     */
    void compareVector3f(sf::Vector3f a, sf::Vector3f b);
};

#endif
