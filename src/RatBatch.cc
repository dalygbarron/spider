#include "RatBatch.hh"
#include "Util.hh"
#include "spdlog/spdlog.h"

RatBatch::RatBatch(sf::Texture const &texture): texture(texture) {
    this->clear();
}

void RatBatch::clear() {
    this->n = 0;
}

void RatBatch::draw(sf::IntRect sprite, sf::Vector2f pos) {
    sf::FloatRect box;
    box.left = pos.x - sprite.width / 2;
    box.top = pos.y - sprite.height / 2;
    box.width = sprite.width;
    box.height = sprite.height;
    this->draw(sprite, box);
}

void RatBatch::draw(
    sf::IntRect sprite,
    sf::Vector2f pos,
    float rot,
    sf::Vector2f scale
) {
    sf::Transform transform;
    transform.translate(pos).rotate(Util::degrees(rot)).scale(scale);
    int nVertices = this->n * 4;
    if (nVertices + 4 > this->vertices.size()) {
        this->vertices.resize(this->vertices.size() + 4);
    }
    this->vertices[nVertices].position = transform.transformPoint(
        -sprite.width / 2,
        -sprite.height / 2
    );
    this->vertices[nVertices].texCoords = sf::Vector2f(sprite.left, sprite.top);
    this->vertices[nVertices].color = sf::Color::White;
    this->vertices[nVertices + 1].position = transform.transformPoint(
        sprite.width / 2,
        -sprite.height / 2
    );
    this->vertices[nVertices + 1].texCoords = sf::Vector2f(
        sprite.left + sprite.width,
        sprite.top
    );
    this->vertices[nVertices + 1].color = sf::Color::White;
    this->vertices[nVertices + 2].position = transform.transformPoint(
        sprite.width / 2,
        sprite.height / 2
    );
    this->vertices[nVertices + 2].texCoords = sf::Vector2f(
        sprite.left + sprite.width,
        sprite.top + sprite.height
    );
    this->vertices[nVertices + 2].color = sf::Color::White;
    this->vertices[nVertices + 3].position = transform.transformPoint(
        -sprite.width / 2,
        sprite.height / 2
    );
    this->vertices[nVertices + 3].texCoords = sf::Vector2f(
        sprite.left,
        sprite.top + sprite.height
    );
    this->vertices[nVertices + 3].color = sf::Color::White;
    this->n++;
}

void RatBatch::draw(sf::IntRect sprite, sf::FloatRect pos) {
    int nVertices = this->n * 4;
    if (nVertices + 4 > this->vertices.size()) {
        this->vertices.resize(this->vertices.size() + 4);
    }
    this->vertices[nVertices].position = sf::Vector2f(pos.left, pos.top);
    this->vertices[nVertices].texCoords = sf::Vector2f(sprite.left, sprite.top);
    this->vertices[nVertices].color = sf::Color::White;
    this->vertices[nVertices + 1].position = sf::Vector2f(
        pos.left + pos.width,
        pos.top
    );
    this->vertices[nVertices + 1].texCoords = sf::Vector2f(
        sprite.left + sprite.width,
        sprite.top
    );
    this->vertices[nVertices + 1].color = sf::Color::White;
    this->vertices[nVertices + 2].position = sf::Vector2f(
        pos.left + pos.width,
        pos.top + pos.height
    );
    this->vertices[nVertices + 2].texCoords = sf::Vector2f(
        sprite.left + sprite.width,
        sprite.top + sprite.height
    );
    this->vertices[nVertices + 2].color = sf::Color::White;
    this->vertices[nVertices + 3].position = sf::Vector2f(
        pos.left,
        pos.top + pos.height
    );
    this->vertices[nVertices + 3].texCoords = sf::Vector2f(
        sprite.left,
        sprite.top + sprite.height
    );
    this->vertices[nVertices + 3].color = sf::Color::White;
    this->n++;
}

void RatBatch::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.texture = &this->texture;
    target.draw(
        this->vertices.data(),
        this->n * 4,
        sf::PrimitiveType::Quads,
        states
    );
}
