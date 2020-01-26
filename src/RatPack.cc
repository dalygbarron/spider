#include "RatPack.hh"
#include "pugixml.hpp"
#include "spdlog/spdlog.h"

sf::Texture &RatPack::getTextureMutable() {
    return this->texture;
}

sf::Texture const &RatPack::getTexture() const {
    return this->texture;
}

sf::FloatRect RatPack::get(const char *name) const {
    if (!this->rats.count(name)) {
        spdlog::error("trying to get missing '{}' from rat pack", name);
        return sf::FloatRect(0, 0, 0, 0);
    }
    return this->rats.at(name);
}

void RatPack::add(const char *name, sf::FloatRect shape) {
    if (this->rats.count(name)) {
        spdlog::error("'{}' is duplicated in rat pack", name);
    }
    this->rats[name] = shape;
}

void RatPack::clear() {
    this->rats.clear();
}
