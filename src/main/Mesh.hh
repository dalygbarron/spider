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
         * Remove a vertex from the mesh.
         * @param index is the index of the vertex to remove.
         */
        void remove(int index);

        /**
         * Tells you the centrepoint of the mesh.
         * @return the centre.
         */
        sf::Vector2f getCentre();

        /**
         * Gives you a mutable pointer to some vertex. Obviously this will not
         * outlive the mesh. DOn't be a retard with it, just use it and then
         * forget it.
         * @param index is the index of the vertex you want.
         * @return a mutable pointer to it, which is null if you gave an
         *         invalid index.
         */
        sf::Vector2f *getVertex(int index);

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
         * Tells you if the given latitude and longitude would be in this shape
         * if this shape was the surface of a sphere.
         * @param coordinate is the location to check.
         * @return true if it is inside and false otherwise.
         */
        int inSphere(sf::Vector2f coordinate) const;

        /**
         * Gives you the index of the vertex closest to the given position.
         * @param pos       is the point to find an edge near.
         * @param threshold is the maximum distance within which to find
         *                  a vertex.
         * @return the index in the vertices of the found vertex, or -1 if none
         *         were particularly close or the mesh has no vertices or
         *         something.
         */
        int getClosestVertex(sf::Vector2f pos, float theshold) const;

    private:
        std::vector<sf::Vector2f> vertices;
};

#endif
