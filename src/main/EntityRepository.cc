#include "Repository.hh"
#include "Const.hh"
#include "pugixml.hpp"

EntityRepository::EntityRepository(RatPack const &spritesheet):
    spritesheet(spritesheet)
{
    // nothing else.
}

Entity *EntityRepository::load(char const *key) const {
    ghc::filesystem::path file = key;
    if (!ghc::filesystem::exists(file)) {
        Entity *entity = new Entity();
        entity->file = file;
        for (int i = 0; i < 3; i++) {
            entity->mesh.addVertex(sf::Vector2f(
                cos(i * Const::DOUBLE_PI / 3) * 50,
                sin(i * Const::DOUBLE_PI / 3) * 50
            ));
        }
        return entity;
    }
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(key);
    if (!result) {
        spdlog::error("couldn't open entity file '{}'", key);
        return NULL;
    }
    pugi::xml_node node = doc.child("entity");
    if (!node) {
        spdlog::error("no entity in alleged entity file '{}'", key);
        return NULL;
    }
    Entity *entity = new Entity();
    entity->file = file;
    entity->name = node.attribute("name").value();
    entity->item = node.attribute("item").value();
    entity->spriteName = node.attribute("rat").value();
    entity->sprite = this->spritesheet.get(entity->spriteName.c_str());
    entity->offset.x = node.attribute("offset-x").as_float();
    entity->offset.y = node.attribute("offset-y").as_float();
    for (pugi::xml_node point = node.child("point"); point;
        point = point.next_sibling("point")
    ) {
        entity->mesh.addVertex(sf::Vector2f(
            point.attribute("x").as_float(),
            point.attribute("y").as_float()
        ));
    }
    return entity;
}
