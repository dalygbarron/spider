#include "Util.hh"
#include "pugixml.hpp"
#include <stdio.h>

sf::Texture *textureFromFile(char const *filename) {
    sf::Texture *texture = new Texture();
    if (!texture->loadFromFile(filename)) {
        fprintf(STDERR, "Couldn't load a pic from '%s'", filename);
        delete texture;
        return NULL;
    }
    return texture;
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
    // load the bits now;

}

Entity *Util::entityFromFile(char const *filename) {

}
