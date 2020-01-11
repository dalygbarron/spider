#ifndef UTIL_H
#define UTIL_H

/**
 * Contains random utility functions for your pleasure.
 */
namespace Util {
    /**
     * Loads a texture from a file and returns it.
     * @param filename is the file name of the texture to open.
     * @return the texture or null if it could not be loaded.
     */
    sf::Texture *textureFromFile(char const *filename);

    /**
     * Loads a level in from the given file.
     * @param filename is the name of the file from which to load.
     * @return the loaded level or null if you gave a bad filename.
     */
    Level *levelFromFile(char const *filename);

    /**
     * Loads an entity in from the given file.
     * @param filename is the name of the file from which to load.
     * @return the loaded entity or null if you gave a bad filename.
     */
    Entity *entityFromFile(char const *filename);
}

#endif
