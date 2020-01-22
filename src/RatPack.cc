#include "RatPack.hh"

RatPack::RatPack(sf::Texture &texture): texture(texture) {
    // does nothing atm.
}

sf::Texture const &RatPack::getTexture() const {
    return this->texture;
}

sf::FloatRect RatPack::getRat(char const *name) const {
    if (!this->rats.count(&name)) {
        fprintf(stderr, "Trying to get missing '%s' from rat pack.\n", name);
        return sf::FloatRect(0, 0, 0, 0);
    }
    return this->rats.at(name);
}

void RatPack::addRat(char const &name, sf::FloatRect shape) {
    if (this->rats.count(&name)) {
        fprintf(stderr, "'%s' is duplicated in rat pack.\n", name);
    }
    this->rats[&name] = shape;
}
