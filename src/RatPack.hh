#ifndef RAT_PACK_H
#define RAT_PACK_H

#include <SFML/Graphics.hpp>
#include <unordered_map>

/**
 * Fits a whole ratpack file of sprites and lets you put those sprites onto
 * quads whenever you so desire.
 */
class RatPack {
    public:
        /**
         * Gives you mutable access to the rat pack's texture so that you can
         * change it and stuff.
         * @return a mutable reference to the texture.
         */
        sf::Texture &getTextureMutable();

        /**
         * Gives you immutable access to the rat pack's texture so you can draw
         * with it in normal use cases.
         * @return the texture it uses.
         */
        sf::Texture const &getTexture() const;

        /**
         * Adds another rat to the rat pack.
         * @param name is the name of the added rat.
         * @param shape is the position and size of the rat within the overall
         *        texture.
         */
        void add(const char *name, sf::FloatRect shape);

        /**
         * Gives you the dimensions of the given rat.
         * @return a rectangle that covers it.
         */
        sf::FloatRect get(const char *name) const;

        /**
         * Gets rid of all the rats.
         */
        void clear();

    private:
        std::unordered_map<std::string, sf::FloatRect> rats;
        sf::Texture texture;
};

#endif
