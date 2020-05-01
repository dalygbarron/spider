#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Rectangle.hh"
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include <SFML/Graphics.hpp>

/**
 * A shader based background that renders at a specified resolution and then
 * draws it to the screen.
 */
class Background {
    public:
        /**
         * Creates the background by setting where it's gonna draw.
         * @param bounds is the area the background will fill.
         */
        Background(Rectangle bounds);

        /**
         * Gets the background ready to draw from a file.
         * @param file is the file containing the shader.
         */
        void initFromFile(char const *file);

        /**
         * Gets the background ready to draw from a string.
         * @param string is the shader text.
         */
        void initFromString(char const *string);

        /**
         * Updates the timer.
         */
        void update();

        /**
         * Draws the background onto the render target.
         * @param target is where to draw the background.
         */
        void draw(sf::RenderTarget &target) const;

        /**
         * Sets the background's texture if it uses one.
         * @param texture is the texture to use.
         */
        void setTexture(sf::Texture const *texture);

        /**
         * Sets a uniform for the backgrounfd shader.
         * @param name  is the name of the uniform to set.
         * @param value is the value to give it.
         */
        void setUniform(char const *name, glm::vec2 const &value);

        /**
         * Sets a uniform for the background shader.
         * @param name  is the name of the uniform to set.
         * @param value is the value to give the uniform.
         */
        void setUniform(char const *name, glm::vec3 const &value);

        /**
         * Sets a colour uniform on the background shader.
         * @oaram name  is the name of the uniform.
         * @param value is the value to set the uniform to.
         */
        void setUniform(char const *name, sf::Color const &value);

        /**
         * Sets a 4x4 matrix uniform on the background shader.
         * @oaram name  is the name of the uniform.
         * @param value is the value to set the uniform to.
         */
        void setUniform(char const *name, glm::mat4 const &value);

        /**
         * Sets a float uniform on the background shader.
         * @oaram name  is the name of the uniform.
         * @param value is the value to set the uniform to.
         */
        void setUniform(char const *name, float value);

    private:
        Rectangle bounds;
        sf::RectangleShape back;
        sf::RectangleShape mask;
        mutable sf::RenderTexture buffer;
        sf::Shader shader;
        int timer = 0;

        /**
         * Resets some uniforms that need to be set in multiple places.
         */
        void resetUniforms();
};

#endif
