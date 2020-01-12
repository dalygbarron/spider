#include "Level.hh"

Level::Level(Picture *picture) {
    this->picture = picture;
}

Level::~Level() {
    delete this->picture;
}

Picture &Level::getPicture() {
    return *(this->picture);
}

void Level::setPicture(Picture *picture) {
    delete this->picture;
    this->picture = picture;
}
