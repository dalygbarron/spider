#include "Entity.hh"

Entity::Entity(Picture *picture) {
    this->picture = picture;
}

Entity::~Entity() {
    if (this->picture) delete this->picture;
}

void Entity::setPicture(Picture *picture) {
    if (this->picture) delete this->picture;
    this->picture = picture;
}

Picture *Entity::getPicture() {
    return this->picture;
}
