#include "Level.hh"
#include "Util.hh"

Level::Level(Picture *picture) {
    this->picture = picture;
}

Level::~Level() {
    delete this->picture;
}

int Level::getClean() const {
    return this->clean;
}

Picture const *Level::getPicture() const {
    return this->picture;
}

void Level::setPicture(Picture *picture) {
    if (this->picture) delete this->picture;
    this->picture = picture;
}
