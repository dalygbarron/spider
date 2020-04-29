#ifndef WORLD_H
#define WORLD_H

#include "Lindel.hh"
#include "Util.hh"
#include "Renderer.hh"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#endif

/**
 * Represents a 3d world that goes behind the main bit of the level in an
 * adventure bit.
 */
class World {
    public:
        /**
         * Constructor sets up some stuff.
         */
        World();

        /**
         * Destructor does opengl stuff.
         */
        ~World();

        /**
         * Sets the shell that goes around the player's view.
         * @param texture is the skybox.
         */
        void setShell(sf::Texture const *texture);

        /**
         * Sets the texture of the grounde plane.
         * @param texture is the texture to draw it with.
         */
        void setPlane(sf::Texture const *texture);

        /**
         * Sets the sky colour at the horizon.
         * @param colour is the colour.
         */
        void setHorizon(sf::Color colour);

        /**
         * Sets the sky colour at the top of the sky.
         * @param colour is the colour.
         */
        void setSky(sf::Color colour);

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
        void addLindel(Entity const &entity, sf::Vector3f position);

    private:
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 gravity;
        glm::vec2 rotation;
        std::vector<Lindel> lindels;
        GLuint vao;
        GLuint skyVB;
        GLuint skyShader;
};

#endif
