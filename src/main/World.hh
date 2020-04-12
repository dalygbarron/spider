#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>

/**
 * Represents a 3d world that goes behind the main bit of the level in an
 * adventure bit.
 */
class World {
    public:
        /**
         * Creates and empty world that has a ground texture and a sky texture.
         * @param ground is the ground texture.
         * @param sky    is the sky texture.
         */
        World(sf::Texture const *ground, sf::Texture const *sky);

        /**
         * Updates the world, and if something has been interacted with then it
         * @return the name of the function to run and the argument to pass it.
         *         Hold no illusions that these string will persist because
         *         they shall not.
         */
        std::pair<char const *, char const *> update(sf::Vector2f camera);

        /**
         * draws the worlde.
         * @param target is the screen to draw on.
         * @param camera is the angle of the camera in latitude and longitude.
         */
        void draw(sf::RenderTarget &target, sf::Vector2f camera) const;

    private:
        sf::Vector3f position;
        sf::Vector3f velocity;
        sf::Vector3f gravity;
        sf::Vector2f rotation;
        sf::Shader shader;
        sf::RectangleShape back;
        std::vector<Fish> fishes;
};

#endif
