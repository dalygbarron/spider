#include "Patch.hh"
#include "spdlog/spdlog.h"

void Patch::fill(
    Rectangle sprite,
    int left,
    int top,
    int right,
    int bottom
) {
    this->bottomLeft = Rectangle(
        sprite.pos + glm::vec2(0, sprite.size.y - bottom),
        glm::vec2(left, bottom)
    );
    this->left = Rectangle(
        sprite.pos + glm::vec2(0, top),
        glm::vec2(left, sprite.size.y - (top + bottom))
    );
    this->topLeft = Rectangle(sprite.pos, glm::vec2(left, top));
    this->top = Rectangle(
        sprite.pos + glm::vec2(left, 0),
        glm::vec2(sprite.size.x - (left + right), top)
    );
    this->topRight = Rectangle(
        sprite.pos + glm::vec2(sprite.size.x - right, 0),
        glm::vec2(right, top)
    );
    this->right = Rectangle(
        sprite.pos + vec2(sprite.size.x - right, top),
        vec2(right, sprite.size.y - (top + bottom))
    );
    this->bottomRight = Rectangle(
        sprite.pos + vec2(sprite.size.x - right, sprite.size.y - bottom),
        vec2(right, bottom)
    );
    this->bottom = Rectangle(
        sprite.pos + glm::vec2(left, sprite.size.y - bottom),
        glm::vec2(sprite.width - (left + right), bottom)
    );
    this->middle = Rectangle(
        sprite.pos + vec2(left, top),
        vec2(sprite.size.x - (left + right), sprite.size.y - (top + bottom))
    );
}
