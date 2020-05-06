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
        static unsigned int const SORT_PHASES = 15;
        constexpr static float const SIGHT_DISTANCE = 1000;

        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 gravity;
        glm::vec2 rotation;

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
            glm::vec2 fov,
            glm::mat4 projection
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
        void addLindel(Entity const *entity, glm::vec3 position);

        /**
         * Adds a behaviour to the map of the world's behaviours that there are
         * in there.
         * @param name      is the name by which lindels will try to find the
         *                  behaviour.
         * @param behaviour is the behaviour to add.
         */
        void addBehaviour(char const *name, Behaviour behaviour);

        /**
         * Find a behaviour that is in the world.
         * @param name is the name of the behaviour to find.
         * @return a pointer to the behaviour if it is found, or null if it is
         *         not found.
         */
        Behaviour const *getBehaviour(char const *name) const;

    private:
        glm::ivec2 size;
        glm::vec2 fov;
        glm::mat4 projection;
        Background background;
        std::vector<Lindel> lindels;
        std::unordered_map<std::string, Behaviour> behaviours;
        int phase = 0;

        /**
         * Makes a lindel do the stuff which lindels are supposed to do with
         * the whole behaviour thing.
         * @param lindel is the lindel to control.
         */
        void lindelBehaviour(Lindel &lindel);
};

#endif
