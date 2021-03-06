#ifndef FILE_IO
#define FILE_IO

#include "Level.hh"
#include "Entity.hh"
#include "Patch.hh"
#include "RatPack.hh"
#include "Core.hh"
#include "World.hh"
#include "pugixml.hpp"
#include <SFML/Graphics.hpp>

/**
 * Contains functions relating to loading and saving stuff to files.
 */
namespace FileIO {
    /**
     * Reads a whole file into a nice big string for you to have fun with.
     * @param path is the path to the file.
     * @return what ws read.
     */
    char const *readFile(ghc::filesystem::path path);

    /**
     * Reads a switch expression from a string in a stack based way where
     * things are put on a stack and then go into the next things etc also if
     * the stack is left with multiple things they are anded together.
     * @param string is the text to parse from.
     * @return the new switch expression which is null if you gave an empty or
     *         bad string.
     */
    Memory::SwitchExpression *parseSwitchExpression(char const *string);

    /**
     * Loads a memory from the file it should be at for you. If there is no
     * such file then it returns a blank memory.
     * @param id is the id of the memory to load.
     */
    Memory loadMemory(int id);

    /**
     * Saveas a memory to the right file for it's id.
     * @param memory is the memory to save.
     */
    void saveMemory(Memory const &memory);

    /**
     * Parses an xml node into a 9patch and put it into the given renderer in
     * the correct spot depending on what role it says it fills.
     * @param core     is the core to put the patch into.
     * @param node     is the node to parse it from.
     */
    void parsePatch(Core &core, pugi::xml_node const &node);

    /**
     * Saves an entity to it's allotted file in the same xml format that this
     * program loads.
     * @param entity is the entity to save.
     */
    void saveEntity(Entity const &entity);

    /**
     * Saves a level to it's allotted file in the same xml format that this
     * program loads.
     * @param level is the level to save.
     */
    void saveLevel(Level const &level);

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
     * @param path       is the path to the file to load. It must exist or it
     *                   will probably crash.
     * @param allowMusic is whether to allow it to play music.
     * @return the new core.
     */
    Core *loadCoreFromFile(ghc::filesystem::path const &path, int allowMusic);

    /**
     * TUrns an xml node into the knob it represents recursively.
     * @param knob        is the xml node.
     * @param spritesheet is the spritesheet to get stuff out of.
     * @return the knob or null if it's no good.
     */
    Knob *parseKnob(pugi::xml_node node, RatPack const &spritesheet);

    /**
     * Parses a 3d world out of xml.
     * @param node is the xml node to load from.
     * @param core gives access to the display params and entity repo.
     * @return the world that was created or null if there was a fuckup.
     */
    World *parseWorld(pugi::xml_node node, Core &core);

    /**
     * Loads an entity out of xml and gives it it's sprite and stuff.
     * @param node is the sml node to read from.
     * @param core is the core thing which provides sprites.
     * @return the loaded entity or null if it could not load it.
     */
    Entity *parseEntity(pugi::xml_node node, Core &core);

    /**
     * Loads a level out of xml.
     * @param node is the xml node read from.
     * @return the loaded level or null if it fucked up.
     */
    Level *parseLevel(pugi::xml_node node);

    /**
     * Screenshots one screen. I know that this means it doesn't do when one
     * screen is on top of another but whatever, I don't need that right now
     * and it's hard to implement so who cares. It saves it to a filename that
     * doesn't exist and that is based on the current time. If the name it made
     * up actually exists somehow it just does nothing.
     * @param screen is the screen to render.
     */
    void screenshot(Screen const &screen);

    /**
     * Loads in XML from a file, then parses it into your desired type using
     * your desired parsing function.
     * @param filename is the name of the file to load.
     * @param parser   is the function used to parse the xml.
     * @return whatever the parser returned, unless the the file would not open
     *         or the xml was invalid in which case it's null for certain.
     */
    template <class T> T *readXmlFile(
        char const *filename,
        T *(*parser)(pugi::xml_node node)
    ) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(filename);
        if (!result) {
            spdlog::error(
                "'{}' does not work as an xml file: {}",
                filename,
                result.description()
            );
            return NULL;
        }
        return parser(doc.first_child());
    }

    /**
     * Reads xml from a string, then parses it into your desired type using
     * your desired parsing function.
     * @param xml    is the string to parse.
     * @param parser is the function used to parse the xml.
     * @return whatever the parser returned, unless the the file would not open
     *         or the xml was invalid in which case it's null for certain.
     */
    template <class T, class A> T *readXml(
        char const *xml,
        T *(*parser)(pugi::xml_node node, A const &arg),
        A const &arg
    ) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(xml);
        if (!result) {
            spdlog::error(
                "'{}' does not work as xml: {}",
                xml,
                result.description()
            );
            return NULL;
        }
        return parser(doc.first_child(), arg);
    }
};

#endif
