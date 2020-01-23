#include "RatPack.hh"
#include "pugixml.hpp"
#include "spdlog/spdlog.h"

int RatPack::loadFromFile(char const *filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    if (!result) {
        spdlog::error(
            "RatPack file '{}' not valid xml: {}",
            filename,
            result.description()
        );
    }
    pugi::xml_node node = doc.child("pack");
    if (!node) {
        spdlog::error("File '{}' is no rat pack", filename);
    }
    spdlog::info(
        "Loading rat pack from '{}' version '{}'",
        filename,
        node.attribute("version").value()
    );
    char const *image = node.attribute("image").value();
    if (!this->texture.loadFromFile(image)) {
        spdlog::error("Could not load '{}' for rat pack", image);
    }
    for (pugi::xml_node rat = node.child("rat"); rat;
        rat = rat.next_sibling("rat")
    ) {
        this->addRat(rat.attribute("name").value(), sf::FloatRect(
            rat.attribute("x").as_int(),
            rat.attribute("y").as_int(),
            rat.attribute("w").as_int(),
            rat.attribute("h").as_int()
        ));
    }
}

sf::Texture const &RatPack::getTexture() const {
    return this->texture;
}

sf::FloatRect RatPack::getRat(char const *name) const {
    if (!this->rats.count(name)) {
        fprintf(stderr, "Trying to get missing '%s' from rat pack.\n", name);
        return sf::FloatRect(0, 0, 0, 0);
    }
    return this->rats.at(name);
}

void RatPack::addRat(char const *name, sf::FloatRect shape) {
    if (this->rats.count(name)) {
        fprintf(stderr, "'%s' is duplicated in rat pack.\n", name);
    }
    this->rats[name] = shape;
}
