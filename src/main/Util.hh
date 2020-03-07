#ifndef UTIL_H
#define UTIL_H

#include "Knob.hh"
#include "sol.hh"
#include <SFML/Graphics.hpp>

/**
 * Contains random utility functions for your pleasure.
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
     * Gives you the length of the given vector.
     * @param vector is the vector to measure.
     * @return the length.
     */
    float length(sf::Vector3f vector);

    /**
     * Tells you if a given latitude is between two other latitudes, under the
     * assumption that the "inner" part between the latitudes is the shorter
     * part.
     * @param a     is the first latitude.
     * @param b     is the second latitude.
     * @param point is the latitude that we are checking is between the other
     *              two.
     */
    int inSlice(float a, float b, float point);

    /**
     * Rotates a latitude and longitude first around the X axis, and then
     * around the Y axis to create a fully new latitude and longitude.
     * @param coordinate is the latitude and longitude to rotate.
     * @param angle  is the two angles to rotate it by.
     * @return the rotated latitude and longitude.
     */
    sf::Vector2f rotate(sf::Vector2f coordinate, sf::Vector2f angle);

    /**
     * Gives you the manhattan distance between two float vectors. It is like
     * normal distance but shitty and fast.
     * @param a is the first position.
     * @param b is the second position.
     * @return the manhattan distance between the two positions.
     */
    float manhattan(sf::Vector2f a, sf::Vector2f b);

    /**
     * Converts a location on the screen to an angle from the centre of
     * a sphere that is around the screen.
     * @param pos    is the position on the screen.
     * @param camera is the rotation of the camera.
     * @return the coordinates of that point on the sphere.
     */
    sf::Vector2f screenToSphere(sf::Vector2f pos, sf::Vector2f camera);

    /**
     * Converts a coordinate on the surface of a sphere to a location on the
     * screen assuming the camera is inside the centre of the sphere.
     * @param coordinate is the latitude and longitude of the point converted.
     * @param camera     is the current angle of the camera.
     * @return the position to place it on the screen. The third dimension is
     *         whether it is appearing on the front of the screen or on the
     *         back of the screen.
     */
    sf::Vector3f sphereToScreen(sf::Vector2f coordinate, sf::Vector2f camera);

    /**
     * Gives you the angle that images should be given thus that they appear to
     * be facing upwards given their position and the angle of the camera.
     * @param camera is the angle of the camera.
     * @param floor  is the screen position of the lowest angle.
     * @param pos    is the position to get an angle for.
     * @return the angle that a pic should get.
     */
    float upAngle(sf::Vector2f camera, sf::Vector2f floor, sf::Vector2f pos);
}

#endif
