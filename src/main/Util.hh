#ifndef UTIL_H
#define UTIL_H

#include "Knob.hh"
#include "Matrix.hh"
#include "sol.hh"
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
     * Gives you a view that makes the screen letterbox so it scales stuff and
     * does blah blah blah you know.
     * @param view       is the view that was currently on.
     * @param dimensions is the new dimensions of the screen.
     * @return the new view to use.
     */
    sf::View getLetterboxView(sf::View view, sf::Vector2i dimensions);

    /**
     * Centres the mouse in the screen.
     * @param window is the window we are using which is needed because the
     *               center of the screen changes depending.
     */
    void centreMouse(sf::Window &window);

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
     * Gives you the distance between two 3d points.
     * @param a is the first point.
     * @param b is the second point.
     * @return the distance between them.
     */
    float distance3(sf::Vector3f a, sf::Vector3f b);

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
    float manhattan3(sf::Vector3f a, sf::Vector3f b);

    /**
     * Converts a 3d position to a location on the surface of a sphere that
     * corresponds to it, with the third parameter being the distance from the
     * centre of the sphere.
     * @param pos is the position to convert.
     * @param camera is the location of the camera.
     * @return the longitude, latitude, and radius of the point from camera.
     */
    sf::Vector3f toSphere(sf::Vector3f pos, sf::Vector3f camera);

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

    /**
     * Rotates a point around another point by a given angle.
     * @param pos    is the point to rotate.
     * @param origin is the point to rotate around.
     * @param angle  is the angle to rotate by.
     * @param scale  is the scale to multiply the final result by.
     * @return the rotated point.
     */
    sf::Vector2f rotateAround(
        sf::Vector2f pos,
        sf::Vector2f origin,
        float angle,
        float scale=1
    );

    /**
     * Converts spherical coordinates to cartesian coordinates.
     * @param spherical is the spherical coordinates to convert.
     * @return the relevant cartesian coordinates assuming that the sphere had
     *         a radius of 1 and was centred at (0, 0, 0).
     */
    sf::Vector3f sphericalToCartesian(sf::Vector2f spherical);

    /**
     * Converts spherical coordinates to cartesian coordinates with the
     * assumption the sphere is centered at (0, 0, 0) and has a radius of 1.
     * @param cartesian is the cartesian coordinates.
     * @return the spherical coordinates.
     */
    sf::Vector2f cartesianToSpherical(sf::Vector3f cartesian);

    /**
     * Creates a transformation matrix that rotates things around the three
     * axes in xyz order.
     * @param c is the three angles of rotation.
     * @return the transformation matrix.
     */
    Matrix cameraToWorldMatrix(sf::Vector3f c);

    /**
     * Transforms a 3d point with a 3x3 matrix since they didn't add it for
     * some stupid reason.
     * @param point is the point to transform.
     * @param c     is the transformation matrix.
     * @return the transformed version.
     */
    sf::Vector3f transformPoint(
        sf::Vector3f point,
        Matrix const &c
    );
};

#endif
