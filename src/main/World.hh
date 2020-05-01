#ifndef WORLD_H
#define WORLD_H

#include "Lindel.hh"
#include "Util.hh"
#include "Background.hh"
#include "Renderer.hh"
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
         * @param size      is the size of the screen which it needs to know.
         * @param fov       is the field of view which it needs to know.
         */
        World(
            sf::Texture const *ground,
            sf::Color horizon,
            sf::Color bottomSky,
            sf::Color topSky,
            float waves,
            glm::ivec2 size,
            glm::vec2 fov
        );

        /**
         * Updates the world, and if something has been interacted with then it
         * @param camera has the matrices for the camera transformation.
         * @return the name of the function to run and the argument to pass it.
         *         Hold no illusions that these string will persist because
         *         they shall not.
         */
        std::pair<char const *, char const *> update(glm::mat4 const &c);

        /**
         * draws the worlde upon thy screene.
         * @param target   is the screen to draw on.
         * @param renderer is used to draw cool shit.
         * @param camera   has the camera transformation matrices.
         */
        void draw(
            sf::RenderTarget &target,
            Renderer &renderer,
            glm::mat4 const &c
        ) const;

        /**
         * Adds a lindel into the world.
         * @param entity   is the entity that the lindel is ripping off.
         * @param position is the position that the lindel will have.
         */
        void addLindel(Entity const &entity, glm::vec3 position);

    private:
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 gravity;
        glm::vec2 rotation;
        Background background;
        std::vector<Lindel> lindels;
};

#endif
