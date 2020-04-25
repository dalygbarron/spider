#include "Background.hh"
#include "Const.hh"
#include "spdlog/spdlog.h"

Background::Background(Rectangle bounds): bounds(bounds) {
    this->buffer.create(bounds.size.x, bounds.size.y);
    this->buffer.setSmooth(true);
    this->back.setSize(sf::Vector2f(bounds.size.x, bounds.size.y));
    this->back.setTextureRect(
        sf::IntRect(bounds.pos.x, bounds.pos.y, bounds.size.x, bounds.size.y)
    );
    this->mask.setPosition(sf::Vector2f(bounds.pos.x, bounds.pos.y));
    this->mask.setSize(sf::Vector2f(bounds.size.x, bounds.size.y));
    this->mask.setTexture(&this->buffer.getTexture());
    this->shader.setUniform("texture", sf::Shader::CurrentTexture);
    this->resetUniforms();
}

void Background::initFromFile(char const *file) {
    if (!this->shader.loadFromFile(file, sf::Shader::Fragment)) {
        spdlog::error(
            "Couldn't read background shader from file '{}'",
            file
        );
    }
    this->resetUniforms();
}

void Background::initFromString(char const *string) {
    if (!this->shader.loadFromMemory(string, sf::Shader::Fragment)) {
        spdlog::error("Couldn't read background shader from string");
    }
    this->resetUniforms();
}

void Background::update() {
    this->timer++;
    this->shader.setUniform("time", this->timer);
}

void Background::draw(sf::RenderTarget &target) const {
    this->buffer.clear(sf::Color::Transparent);
    sf::RenderStates states;
    states.shader = &this->shader;
    this->buffer.draw(this->back, states);
    this->buffer.display();
    target.draw(this->mask);
}

void Background::setTexture(sf::Texture const *texture) {
    this->back.setTexture(texture, true);
    this->resetUniforms();
}

void Background::setUniform(char const *name, sf::Vector2f const &value) {
    this->shader.setUniform(name, (sf::Glsl::Vec2)value);
}

void Background::setUniform(char const *name, sf::Vector3f const &value) {
    this->shader.setUniform(name, value);
}

void Background::setUniform(char const *name, sf::Color const &value) {
    this->shader.setUniform(name, (sf::Glsl::Vec4 const &)value);
}

void Background::resetUniforms() {
    this->shader.setUniform(
        "offset",
        sf::Glsl::Vec2(bounds.pos.x, bounds.pos.y)
    );
    this->shader.setUniform(
        "resolution",
        sf::Glsl::Vec2(bounds.size.y, bounds.size.x)
    );
    this->shader.setUniform("time", this->timer);
}
