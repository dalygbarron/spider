#include "Patch.hh"
#include "spdlog/spdlog.h"

void Patch::fill(
    sf::IntRect sprite,
    int left,
    int top,
    int right,
    int bottom
) {
    this->bottomLeft = sf::IntRect(
        sprite.left,
        sprite.top + sprite.height - bottom,
        left,
        bottom
    );
    this->left = sf::IntRect(
        sprite.left,
        sprite.top + top,
        left,
        sprite.height - (top + bottom)
    );
    this->topLeft = sf::IntRect(
        sprite.left,
        sprite.top,
        left,
        top
    );
    this->top = sf::IntRect(
        sprite.left + left,
        sprite.top,
        sprite.width - (left + right),
        top
    );
    this->topRight = sf::IntRect(
        sprite.left + sprite.width - right,
        sprite.top,
        right,
        top
    );
    this->right = sf::IntRect(
        sprite.left + sprite.width - right,
        sprite.top + top,
        right,
        sprite.height - (top + bottom)
    );
    this->bottomRight = sf::IntRect(
        sprite.left + sprite.width - right,
        sprite.top + sprite.height - bottom,
        right,
        bottom
    );
    this->bottom = sf::IntRect(
        sprite.left + left,
        sprite.top + sprite.height - bottom,
        sprite.width - (left + right),
        bottom
    );
    this->middle = sf::IntRect(
        sprite.left + left,
        sprite.top + top,
        sprite.width - (left + right),
        sprite.height - (top + bottom)
    );
}
