#include "Picture.hh"

Picture::Picture(sf::Texture *texture, char const *filename) {
    this->texture = texture;
    this->filename = filename;
}

Picture::~Picture() {
    delete this->texture;
}
