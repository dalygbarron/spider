#include "RatPack.hh"
#include "pugixml.hpp"
#include "spdlog/spdlog.h"

sf::Texture &RatPack::getTextureMutable() {
    return this->texture;
}

sf::Texture const &RatPack::getTexture() const {
    return this->texture;
}

Rectangle RatPack::get(const char *name) const {
    if (!this->rats.count(name)) {
        spdlog::error("trying to get missing '{}' from rat pack", name);
        return Rectangle();
    }
    return this->rats.at(name);
}

void RatPack::add(const char *name, Rectangle shape) {
    if (this->rats.count(name)) {
        spdlog::error("'{}' is duplicated in rat pack", name);
    }
    this->rats[name] = shape;
}

void RatPack::clear() {
    this->rats.clear();
}

int RatPack::count() const {
    return this->rats.size();
}

std::unordered_map<std::string, Rectangle>::const_iterator RatPack::begin(
) const {
    return this->rats.begin();
}

std::unordered_map<std::string, Rectangle>::const_iterator RatPack::end(
) const {
    return this->rats.end();
}
