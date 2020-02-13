#include "RatPack.hh"
#include "pugixml.hpp"
#include "spdlog/spdlog.h"

sf::Texture &RatPack::getTextureMutable() {
    return this->texture;
}

sf::Texture const &RatPack::getTexture() const {
    return this->texture;
}

sf::IntRect RatPack::get(const char *name) const {
    if (!this->rats.count(name)) {
        spdlog::error("trying to get missing '{}' from rat pack", name);
        return sf::IntRect(0, 0, 0, 0);
    }
    return this->rats.at(name);
}

void RatPack::add(const char *name, sf::IntRect shape) {
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


std::unordered_map<std::string, sf::IntRect>::const_iterator RatPack::begin(
) const {
    return this->rats.begin();
}

std::unordered_map<std::string, sf::IntRect>::const_iterator RatPack::end(
) const {
    return this->rats.end();
}
