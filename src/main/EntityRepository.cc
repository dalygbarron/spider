#include "Repository.hh"
#include "Const.hh"
#include "pugixml.hpp"
#include "glm/vec2.hpp"

EntityRepository::EntityRepository(RatPack const &spritesheet):
    spritesheet(spritesheet)
{
    // nothing else.
}

Entity *EntityRepository::create(ghc::filesystem::path const &path) const {
    Entity *entity = new Entity();
    entity->file = path;
    for (int i = 0; i < 3; i++) {
        entity->mesh.addVertex(glm::vec2(
            cos(i * Const::DOUBLE_PI / 3) * 50,
            sin(i * Const::DOUBLE_PI / 3) * 50
        ));
    }
    return entity;
}

char const *EntityRepository::getNodeName() const {
    return "entity";
}

Entity *EntityRepository::parse(
    pugi::xml_node const &node,
    ghc::filesystem::path const &path
) const {
    Entity *entity = new Entity();
    entity->file = path;
    entity->name = node.attribute("name").value();
    entity->item = node.attribute("item").value();
    entity->itemKey = "_";
    entity->itemKey += node.attribute("item").value();
    entity->spriteName = node.attribute("rat").value();
    entity->sprite = this->spritesheet.get(entity->spriteName.c_str());
    entity->offset.x = node.attribute("offset-x").as_float();
    entity->offset.y = node.attribute("offset-y").as_float();
    entity->scale = node.attribute("scale").as_float();
    // Load mesh points.
    for (pugi::xml_node point = node.child("point"); point;
        point = point.next_sibling("point")
    ) {
        entity->mesh.addVertex(glm::vec2(
            point.attribute("x").as_float(),
            point.attribute("y").as_float()
        ));
    }
    return entity;
}
