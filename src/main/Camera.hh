#ifndef CAMERA_H
#define CAMERA_H

#include "Matrix.hh"
#include <SFML/Graphics.hpp>

/**
 * Basically just stores and calculates 3d camera related matrices.
 */
class Camera {
    public:
        /**
         * Creates the camera and initialises it to (0, 0).
         */
        Camera();

        /**
         * Sets the direction that the camera is looking at and updates
         * matrices.
         * @param longitude is the angle around the y axis the camera should
         *                  have.
         * @param latitude  is the angle to which the camera is raised.
         */
        void set(float longitude, float latitude);

        /**
         * Sets the direction that the camera is looking at and updates
         * matrices.
         * @param coordinate it the longitude and latitude in that order.
         */
        void set(sf::Vector2f coordinate);

        /**
         * Adds some values to the camera's rotation.
         * @param longitude is the extra longitude to add.
         * @param latitude is the extra latitude to add.
         */
        void add(float longitude, float latitude);
        
        /**
         * Gives you the angle the camera is pointing in.
         * @return the longitude and latitude in that ordeer.
         */
        sf::Vector2f getAngle() const;

        /**
         * Gives you the matrix to convert into this camera's coordinates.
         * @return the transformatino matrix.
         */
        Matrix const &toMatrix() const;

        /**
         * Gives you the matrix to convert out of this camera's coordinates.
         * @return the transformatino matrix.
         */
        Matrix const &fromMatrix() const;

    private:
        sf::Vector2f angle;
        Matrix to;
        Matrix from;
};

#endif
