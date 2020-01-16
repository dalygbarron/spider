#ifndef UTIL_H
#define UTIL_H

#include "Picture.hh"
#include "Level.hh"
#include "Entity.hh"
#include "pugixml.hpp"

/**
 * Contains random utility functions for your pleasure.
 */
namespace Util {
    /**
     * Parses an xml node into a level.
     * @param node is the top level node of the level.
     * @return the parsed level or null if it could not be done.
     */
    Level *parseLevel(pugi::xml_node &node);

    /**
     * Parses an xml node into an entity.
     * @param node is the top level node of the entity.
     * @return the parsed entity or null if it could not be done.
     */
    Entity *parseEntity(pugi::xml_node &node);

    /**
     * Loads a picture from a file and returns it.
     * @param filename is the file name of the texture to open.
     * @return the picture or null if it could not be loaded.
     */
    Picture *pictureFromFile(ghc::filesystem::path &path);

    /**
     * Loads a level in from the given file, and if the file does not exist
     * then it creates a new blank level that uses that file.
     * @param filename is the name of the file from which to load.
     * @return the loaded level or null if you gave a bad filename.
     */
    Level *levelFromFile(ghc::filesystem::path &path);

    /**
     * Loads an entity in from the given file, and if the file does not exist
     * then it creates a new blank entity that uses that file.
     * @param filename is the name of the file from which to load.
     * @return the loaded entity or null if you gave a bad filename.
     */
    Entity *entityFromFile(ghc::filesystem::path &path);

    /**
     * Gives you a view that makes the screen letterbox so it scales stuff and
     * does blah blah blah you know.
     * @param view       is the view that was currently on.
     * @param dimensions is the new dimensions of the screen.
     * @return the new view to use.
     */
    sf::View getLetterboxView(sf::View view, sf::Vector2i dimensions);
}

#endif
