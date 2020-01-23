#ifndef UTIL_H
#define UTIL_H

#include "Level.hh"
#include "Entity.hh"
#include "RatPack.hh"
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
     * Parses rats out of an xml node and puts them in the given ratpack.
     * Yes I am aware this is weird since you must have already given it
     * a texture but also I do not really care ha ha ha hah ah.
     * @param
     */
    void parseRats(RatPack &pack, pugi::xml_node &node);

    /**
     * Opens up a rat pack xml file and adds all the nodes from it into a rat
     * pack object.
     * @param pack is the existing pack to add the rats to.
     * @param path is the path at which to load the ratpack xml file.
     */
    void addRatsFromFile(RatPack &pack, ghc::filesystem::path &path);

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
