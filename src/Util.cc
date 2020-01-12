#include "Util.hh"
#include "pugixml.hpp"
#include <stdio.h>

Picture *Util::pictureFromFile(char const *filename) {
    sf::Texture *texture = new Texture();
    if (!texture->loadFromFile(filename)) {
        fprintf(STDERR, "Couldn't load a pic from '%s'", filename);
        delete texture;
        return NULL;
    }
    return new Picture(texture, filename);
}

Entity *Util::entityFromFile(char const *filename) {
    // TODO: this.
    return NULL;
}

Level *Util::levelFromFile(char const *filename) {
    pugi::xml_doc doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    if (!result) {
        fprintf(STDERR, "File '%s' simply can NOT be opened.", filename);
        return NULL;
    }
    pugi::xml_node node = doc.child("level");
    if (!node) {
        fprintf(STDERR, "File '%s' sadly does not contain a level.", filename);
    }
    // Main bits.
    Level *level = new Level(
        pictureFromFile(node.attribute("picture").value())
    );
    level->name = filename;
    level->script = node.attribute("script").value();
    // Defines.
    pugi::xml_node defines = node.child("defines");
    for (pugi::xml_node define = defines.child("define"); define;
        define = define.next_sibling("define")
    ) {
        
    }
    // Entities.

}

Entity *Util::entityFromFile(char const *filename) {

}
