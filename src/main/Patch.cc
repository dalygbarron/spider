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
        sprite.pos + glm::ivec2(0, sprite.size.y - bottom),
        glm::ivec2(left, bottom)
    );
    this->left = Rectangle(
        sprite.pos + glm::ivec2(0, top),
        glm::ivec2(left, sprite.size.y - (top + bottom))
    );
    this->topLeft = Rectangle(sprite.pos, glm::vec2(left, top));
    this->top = Rectangle(
        sprite.pos + glm::ivec2(left, 0),
        glm::ivec2(sprite.size.x - (left + right), top)
    );
    this->topRight = Rectangle(
        sprite.pos + glm::ivec2(sprite.size.x - right, 0),
        glm::ivec2(right, top)
    );
    this->right = Rectangle(
        sprite.pos + glm::ivec2(sprite.size.x - right, top),
        glm::ivec2(right, sprite.size.y - (top + bottom))
    );
    this->bottomRight = Rectangle(
        sprite.pos + glm::ivec2(sprite.size.x - right, sprite.size.y - bottom),
        glm::ivec2(right, bottom)
    );
    this->bottom = Rectangle(
        sprite.pos + glm::ivec2(left, sprite.size.y - bottom),
        glm::ivec2(sprite.size.x - (left + right), bottom)
    );
    this->middle = Rectangle(
        sprite.pos + glm::ivec2(left, top),
        glm::ivec2(sprite.size.x - (left + right), sprite.size.y - (top + bottom))
    );
}
