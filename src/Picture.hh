#ifndef PICTURE_H
#define PICTURE_H

/**
 * Represents an sfml texture and the relative path from which it was loaded.
 */
class Picture {
    public:
        sf::Texture *texture;
        char const *filename;

        /**
         * Creates the picture by passing in it's texture and it's filename.
         * The reason that the texture is passed in is that if the loading of
         * it fails then this object should not be created.
         * @param texture  is the texture of the picture. It will from
         *                 henceforth belong to this object and will be deleted
         *                 when it is.
         * @param filename is the filename of the texture to load.
         */
        Picture(sf::Texture *texture, char const *filename);

        /**
         * Deletes the texture.
         */
        ~Picture();
};

#endif
