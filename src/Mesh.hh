#ifndef MESH_H
#define MESH_H

#include <SFML/Graphics.hpp>

/**
 * Represents a concave polygon.
 */
class Mesh {
    public:
        /**
         * Adds a vertex to the mesh.
         * @param vertex is the vertex to add.
         */
        void addVertex(sf::Vector2f vertex);

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

    private:
        std::vector<sf::Vector2f> vertices;
};

#endif
