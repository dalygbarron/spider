#include "Rectangle.hh"

Rectangle::Rectangle() {
    // do nothing.
}

Rectangle::Rectangle(int x, int y, int w, int h) {
    this->pos.x = x;
    this->pos.y = y;
    this->size.x = w;
    this->size.y = h;
}

Rectangle::Rectangle(glm::ivec2 pos, glm::ivec2 size) {
    this->pos = pos;
    this->size = size;
}

bool Rectangle::contains(glm::vec2 point) const {
    return point.x >= this->pos.x && point.x <= this->pos.x + this->size.x &&
       point.y >= this->pos.y && point.y <= this->pos.y + this->size.y; 
}

glm::vec2 Rectangle::clamp(glm::vec2 point) const {
    if (point.x < this->pos.x) point.x = this->pos.x;
    if (point.y < this->pos.y) point.y = this->pos.y;
    if (point.x >= this->pos.x + this->size.x) {
        point.x = this->pos.x + this->size.x;
    }
    if (point.y >= this->pos.y + this->size.y) {
        point.y = this->pos.y + this->size.y;
    }
    return point;
}
