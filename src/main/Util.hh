#ifndef UTIL_H
#define UTIL_H

#include "Knob.hh"
#include "sol.hh"
#include "glm/gtc/matrix_transform.hpp"
#include <SFML/Graphics.hpp>

/**
 * Contains random utility functions for your pleasure.
 */
namespace Util {
    /**
     * Sleep for the given armound of time.
     * @param delta is the amount of time to sleep for.
     */
    void sleep(double delta);

    /**
     * Finds at what index if any a given string appears in an array of
     * strings. Also, it should be case insensitive but I have not bothered to
     * do that yet.
     * @param string is the string to look for.
     * @param in     is the array to look in.
     * @param n      is the number of strings in the array.
     * @return the index of the string in the array if it is in there, and -1
     *         if it is not in there.
     */
    int findString(char const *string, char const * const *in, int n);

    /**
     * Gives you a view that makes the screen letterbox so it scales stuff and
     * does blah blah blah you know.
     * @param view       is the view that was currently on.
     * @param dimensions is the new dimensions of the screen.
     * @return the new view to use.
     */
    sf::View getLetterboxView(sf::View view, sf::Vector2i dimensions);

    /**
     * Gives you the current rendering window's size as a proportion of the
     * logical screen size.
     * @param window is the rendering window to get the size of.
     * @param size   is the logical size.
     * @return the real window size as a proportion of the logical size in each
     *         dimension.
     */
    glm::vec2 screenScale(sf::Window const &window, glm::ivec2 size);

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
     * Takes a position and clamps that position within the rectangle axis by
     * axis so angle is not relevant.
     * @param point is the point to clamp.
     * @param rect  is the rectangle within to clamp it.
     * @return the clamped version of the point.
     */
    sf::Vector2f clampInRect(sf::Vector2f point, sf::FloatRect rect);

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
     * Gives you the manhattan distance between two 3d points.
     * @param a is the first point.
     * @param b is the second point.
     * @return the manhattan distance between the two 3d points.
     */
    float manhattan3(glm::vec3 a, glm::vec3 b);

    /**
     * Gives you the angle that images should be given thus that they appear to
     * be facing upwards given their position and the angle of the camera.
     * @param camera is the angle of the camera.
     * @param floor  is the screen position of the lowest angle.
     * @param pos    is the position to get an angle for.
     * @return the angle that a pic should get.
     */
    float upAngle(sf::Vector2f camera, sf::Vector2f floor, sf::Vector2f pos);

    /**
     * Rotates a point around another point by a given angle.
     * @param pos    is the point to rotate.
     * @param origin is the point to rotate around.
     * @param angle  is the angle to rotate by.
     * @param scale  is the scale to multiply the final result by.
     * @return the rotated point.
     */
    glm::vec2 rotateAround(
        glm::vec2 pos,
        glm::vec2 origin,
        float angle,
        float scale=1
    );

    /**
     * Creates a camera matrix based on a latitude and longitude angle.
     * @param angle is the two angles with longitude being first.
     * @return camera matrix.
     */
    glm::mat4 camera(glm::vec2 angle);

    /**
     * Converts spherical coordinates to cartesian coordinates.
     * @param spherical is the spherical coordinates to convert.
     * @return the relevant cartesian coordinates assuming that the sphere had
     *         a radius of 1 and was centred at (0, 0, 0).
     */
    glm::vec3 sphericalToCartesian(glm::vec2 spherical);

    /**
     * Converts spherical coordinates to cartesian coordinates with the
     * assumption the sphere is centered at (0, 0, 0) and has a radius of 1.
     * @param cartesian is the cartesian coordinates.
     * @return the spherical coordinates.
     */
    glm::vec2 cartesianToSpherical(glm::vec3 cartesian);

    /**
     * Takes a point on the screen and converts it to spherical
     * coordinates.
     * @param screen is the position on the screen which should be normalised
     *               to be between -0.5 and 0.5.
     * @param camera is the camera rotation matrix.
     * @param projection is the projection matrix.
     */
    glm::vec2 screenToSpherical(
        glm::vec2 screen,
        glm::mat4 camera,
        glm::mat4 projection
    );
};

#endif
