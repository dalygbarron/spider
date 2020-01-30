#ifndef UTIL_H
#define UTIL_H

#include "Level.hh"
#include "Entity.hh"
#include "Patch.hh"
#include "RatPack.hh"
#include "Core.hh"
#include "pugixml.hpp"

/**
 * Contains random utility functions for your pleasure.
 * TODO: so much of this is loading stuff in, maybe I should have a different
 *       file just for loading related stuff.
 */
namespace Util {
    /**
     * Parses an xml node into a 9patch and put it into the given renderer in
     * the correct spot depending on what role it says it fills.
     * @param core     is the core to put the patch into.
     * @param node     is the node to parse it from.
     */
    void parsePatch(Core &core, pugi::xml_node const &node);

    /**
     * Parses an xml node into a level.
     * @param node is the top level node of the level.
     * @return the parsed level or null if it could not be done.
     */
    Level *parseLevel(pugi::xml_node const &node);

    /**
     * Parses an xml node into an entity.
     * @param node is the top level node of the entity.
     * @return the parsed entity or null if it could not be done.
     */
    Entity *parseEntity(pugi::xml_node const &node);

    /**
     * Loads a level in from the given file, and if the file does not exist
     * then it creates a new blank level that uses that file.
     * @param filename is the name of the file from which to load.
     * @return the loaded level or null if you gave a bad filename.
     */
    Level *levelFromFile(ghc::filesystem::path const &path);

    /**
     * Loads an entity in from the given file, and if the file does not exist
     * then it creates a new blank entity that uses that file.
     * @param filename is the name of the file from which to load.
     * @return the loaded entity or null if you gave a bad filename.
     */
    Entity *entityFromFile(ghc::filesystem::path const &path);

    /**
     * Opens up a rat pack xml file and adds all the nodes from it into a rat
     * pack object. Also if it has existing stuff that will be cleared.
     * @param pack is the existing pack to add the rats to.
     * @param path is the path at which to load the ratpack xml file.
     */
    void initRatPackFromFile(RatPack &pack, ghc::filesystem::path const &path);

    /**
     * Opens up a game xml file and adds all the info in it to the given core
     * objct, including loading a ratpack if necessary.
     * @param path is the path to the file to load.
     * @return the new core.
     */
    Core *loadCoreFromFile(ghc::filesystem::path const &path);

    /**
     * Gives you a view that makes the screen letterbox so it scales stuff and
     * does blah blah blah you know.
     * @param view       is the view that was currently on.
     * @param dimensions is the new dimensions of the screen.
     * @return the new view to use.
     */
    sf::View getLetterboxView(sf::View view, sf::Vector2i dimensions);

    /**
     * Converts radians to degrees.
     * @param radians is the amount of radians to convert.
     * @return that amount in degrees.
     */
    float degrees(float radians);
}

#endif
