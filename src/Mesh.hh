#ifndef MESH_H
#define MESH_H

#include <SFML/Graphics.hpp>

/**
 * Represents a concave polygon.
 * TODO: consider adding a bounding box which is calculated as vertices are
 *       added so that checks outside that box can be summarily executed. Could
 *       be beneficial if we have got a few complex meshes in a level, though
 *       my checks seem to suggest it's pretty good performance wise already,
 *       especially since we are only going to be checking collisions on the
 *       single mouse cursor.
 */
class Mesh {
    public:
        static int const CLOSE = 100;

        /**
         * Adds a vertex to the mesh.
         * @param vertex is the vertex to add.
         */
        void addVertex(sf::Vector2f vertex);

        /**
         * Adds a vertex in after the given vertex that is halfway between it
         * and the one after it.
         * @param index is the index of the vertex to split after. If this is
         *              the index of the last node it splits between it and the
         *              first node. If it is not the index of a node nothing
         *              happens.
         */
        void split(int index);

        /**
         * Gives you the vertices in the mesh.
         * @return a constant vector of them.
         */
        std::vector<sf::Vector2f> const &getVertices() const;

        /**
         * Tells you if the given position is in this shape.
         * @param pos is the position to check.
         * @return true if it is inside and false if not.
         */
        int in(sf::Vector2f pos) const;

        /**
         * Gives you the index of the closest vertex to the given point.
         * @param pos is the point to find a vertex near.
         * @return the index in the vertices of the found vertex. There will
         *         always be one no matter what. If you call this on a mesh
         *         with no vertices it will return 0.
         */
        int getClosestVertex(sf::Vector2f pos) const;

    private:
        std::vector<sf::Vector2f> vertices;
};

#endif
