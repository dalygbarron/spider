#ifndef PICTURE_H
#define PICTURE_H

#include "filesystem.hh"
#include <SFML/Graphics.hpp>

/**
 * Represents an sfml texture and the relative path from which it was loaded.
 */
class Picture {
    public:
        ghc::filesystem::path path;

        /**
         * Creates the picture by passing in it's texture and it's filename.
         * The reason that the texture is passed in is that if the loading of
         * it fails then this object should not be created.
         * @param texture  is the texture of the picture. It will from
         *                 henceforth belong to this object and will be deleted
         *                 when it is.
         * @param path     is the path to where it was loaded from.
         */
        Picture(sf::Texture *texture, ghc::filesystem::path &path);

        /**
         * Deletes the texture.
         */
        ~Picture();

        /**
         * Gives you a reference to this picture's texture.
         * @return a reference to the texture so that you know it is not null.
         */
        sf::Texture const &getTexture() const;

    private:
        sf::Texture *texture;
};

#endif
