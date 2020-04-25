#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "glm/vec2.hpp"

/**
 * Represents a rectangle.
 */
class Rectangle {
    public:
        glm::ivec2 pos;
        glm::ivec2 size;

        /**
         * Default constructor creating a rectangle at origin with no
         * dimensions.
         */
        Rectangle();

        /**
         * Creates the rectangle by specifying each number individually.
         * @param x is the distance from the left.
         * @param y is the distance from the top.
         * @param w is the width.
         * @param h is the height.
         */
        Rectangle(int x, int y, int w, int h);

        /**
         * Creates the rectangle by specifying two 2d vectors.
         * @param pos  is the position of the rectangle.
         * @param size is the width and height of the rectangle.
         */
        Rectangle(glm::ivec2 pos, glm::ivec2 size);

        /**
         * Tells you if the given point is within this rectangle.
         * @param point is the point to check.
         * @return true if it's in there and false otherwise.
         */
        bool contains(glm::vec2 point) const;

        /**
         * Takes a point and clamps it within this rectangle so like it's not
         * escaping the boundaries but each dimension is otherwise as close as
         * possible to the original.
         * @param point is the point to clamp.
         * @return the clamped point.
         */
         glm::vec2 clamp(glm::vec2 point) const;
};

#endif
