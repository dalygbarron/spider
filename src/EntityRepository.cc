#include "Repository.hh"
#include "pugixml.hpp"

Entity *EntityRepository::load(char const *key) const {
    ghc::filesystem::path file = key;
    if (!ghc::filesystem::exists(file)) {
        Entity *entity = new Entity();
        entity->file = file;
        return entity;
    }
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(key);
    if (!result) {
        spdlog::error("couldn't open entity file '{}'", key);
        return NULL;
    }
    pugi::xml_node = doc.child("entity");
    if (!node) {
        spdlog::error("no entity in alleged entity file '{}'", key);
        return NULL;
    }
    Entity *entity = new Entity();
    entity->file = file;
    entity->name = node.attribute("name").value();
    entity->itemType = node.attribute("item").value();
    entity->sprite = node.attribute("sprite").value();
    entity->origin.x = node.attribute("origin-x").as_float();
    entity->origin.y = node.attribute("origin-y").as_float();
    // TODO: load in the points of the web.
    return entity;
}
