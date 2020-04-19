#ifndef WORLD_H
#define WORLD_H

#include "Lindel.hh"
#include "Util.hh"
#include "Renderer.hh"
#include "Background.hh"
#include <SFML/Graphics.hpp>

/**
 * Represents a 3d world that goes behind the main bit of the level in an
 * adventure bit.
 */
class World {
    public:
        /**
         * Creates and empty world that has a ground texture and a sky texture.
         * @param ground    is the ground texture.
         * @param bottomSky is the colour to put at the bottom of the sky
         *                  gradient.
         * @param topSky    is the colour to put at the top of the sky.
         */
        World(
            sf::Texture const *ground,
            sf::Color horizon,
            sf::Color bottomSky,
            sf::Color topSky
        );

        /**
         * Updates the world, and if something has been interacted with then it
         * @param camera is a 4x4 matrix converting from the rotated camera to
         *               world coordinates.
         * @return the name of the function to run and the argument to pass it.
         *         Hold no illusions that these string will persist because
         *         they shall not.
         */
        std::pair<char const *, char const *> update(Matrix const &camera);

        /**
         * draws the worlde upon thy screene.
         * @param target   is the screen to draw on.
         * @param renderer is used to draw cool shit.
         * @param camera   is the 4x4 camera to world matrix.
         */
        void draw(
            sf::RenderTarget &target,
            Renderer &renderer,
            Matrix const &camera
        ) const;

        /**
         * Adds a lindel into the world.
         * @param entity   is the entity that the lindel is ripping off.
         * @param position is the position that the lindel will have.
         */
        void addLindel(Entity const &entity, sf::Vector3f position);

    private:
        sf::Vector3f position;
        sf::Vector3f velocity;
        sf::Vector3f gravity;
        sf::Vector2f rotation;
        sf::Shader shader;
        Background background;
        std::vector<Lindel> lindels;
};

#endif
