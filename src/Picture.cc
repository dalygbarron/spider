#include "Picture.hh"

Picture::Picture(sf::Texture *texture, ghc::filesystem::path &path) {
    this->texture = texture;
    this->path = path;
}

Picture::~Picture() {
    delete this->texture;
}

sf::Texture const &Picture::getTexture() const {
    return *(this->texture);
}
