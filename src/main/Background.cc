#include "Background.hh"
#include "Const.hh"
#include "spdlog/spdlog.h"

Background::Background(sf::IntRect bounds) {
    this->bounds = bounds;
    this->buffer.create(bounds.width, bounds.height);
    this->buffer.setSmooth(true);
    this->back.setSize(sf::Vector2f(bounds.width, bounds.height));
    this->back.setTextureRect(bounds);
    this->mask.setPosition(sf::Vector2f(bounds.left, bounds.top));
    this->mask.setSize(sf::Vector2f(bounds.width, bounds.height));
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

void Background::setUniform(char const *name, Matrix const &value) {
    float values[16];
    values[0] = value.content[0][0];
    values[1] = value.content[0][1];
    values[2] = value.content[0][2];
    values[3] = value.content[0][3];
    values[4] = value.content[1][0];
    values[5] = value.content[1][1];
    values[6] = value.content[1][2];
    values[7] = value.content[1][3];
    values[8] = value.content[2][0];
    values[9] = value.content[2][1];
    values[10] = value.content[2][2];
    values[11] = value.content[2][3];
    values[12] = value.content[3][0];
    values[13] = value.content[3][1];
    values[14] = value.content[3][2];
    values[15] = value.content[3][3];
    sf::Glsl::Mat4 glslMatrix(values);
    this->shader.setUniform(name, glslMatrix);
}

void Background::resetUniforms() {
    this->shader.setUniform(
        "offset",
        sf::Glsl::Vec2(bounds.left, bounds.top)
    );
    this->shader.setUniform(
        "resolution",
        sf::Glsl::Vec2(bounds.width, bounds.height)
    );
    this->shader.setUniform("time", this->timer);
}
