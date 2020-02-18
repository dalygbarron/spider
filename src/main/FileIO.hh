#ifndef FILE_IO
#define FILE_IO

#include "Level.hh"
#include "Entity.hh"
#include "Patch.hh"
#include "RatPack.hh"
#include "Core.hh"
#include "pugixml.hpp"
#include <SFML/Graphics.hpp>

/**
 * Contains functions relating to loading and saving stuff to files.
 */
namespace FileIO {
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
     * Loads a level in from the given file, and if the file does not exist
     * then it creates a new blank level that uses that file.
     * @param filename is the name of the file from which to load.
     * @return the loaded level or null if you gave a bad filename.
     */
    Level *levelFromFile(ghc::filesystem::path const &path);

    /**
     * Saves an entity to the given file in the same xml format that this
     * program loads.
     * @param entity is the entity to save.
     */
    void saveEntity(Entity const &entity);

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
};

#endif
