#include "Background.hh"
#include "Const.hh"
#include "spdlog/spdlog.h"

Background::Background(sf::IntRect bounds) {
    this->bounds = bounds;
    this->buffer.create(bounds.width, bounds.height);
    this->back.setSize(sf::Vector2f(bounds.width, bounds.height));
    this->back.setTextureRect(bounds);
    this->mask.setPosition(sf::Vector2f(bounds.left, bounds.top));
    this->mask.setSize(sf::Vector2f(bounds.width, bounds.height));
    this->mask.setTexture(&this->buffer.getTexture());
}

void Background::initFromFile(char const *file) {
    if (!this->shader.loadFromFile(file, sf::Shader::Fragment)) {
        spdlog::error(
            "Couldn't read background shader from file '{}'",
            file
        );
    }
    this->shader.setUniform(
        "resolution",
        sf::Glsl::Vec2(bounds.width, bounds.height)
    );
    this->shader.setUniform("time", this->timer);
}

void Background::initFromString(char const *string) {
    if (!this->shader.loadFromMemory(string, sf::Shader::Fragment)) {
        spdlog::error("Couldn't read background shader from string");
    }
    this->shader.setUniform(
        "resolution",
        sf::Glsl::Vec2(bounds.width, bounds.height)
    );
    this->shader.setUniform("time", this->timer);
}

void Background::update() {
    this->timer++;
    this->shader.setUniform("time", this->timer);
}

void Background::draw(sf::RenderTarget &target) const {
    sf::RenderStates states;
    states.shader = &this->shader;
    this->buffer.draw(this->back, states);
    this->buffer.display();
    target.draw(this->mask);
}

void Background::setTexture(sf::Texture const *texture) {
    this->back.setTexture(texture);
    this->shader.setUniform("texture", sf::Shader::CurrentTexture);
}

void Background::setUniform(char const *name, sf::Vector2f value) {
    this->shader.setUniform(name, (sf::Glsl::Vec2)value);
}
