#include "Rat.hh"

Rat::Rat(sf::IntRect sprite, sf::Vector2u dimensions) {
    this->sprite = sprite;
    this->dimensions = dimensions;
}

void Rat::update() {
    if (this->current && this->rolling) this->timer++;
}

sf::IntRect Rat::getFrame() {
    int index;
    if (!this->current) {
        index = 0;
    } else if (this->current->loop) {
        index = (this->timer / this->current->frameTime) %
            this->current->frames.size();
    } else {
        index = min(
            this->timer / this->current->frameTime,
            this->current->frames.size()
        );
    }
    return sf::IntRect(
        (index % this->dimensions.x) * this->rat.width / this->dimensions.x,
        (index / this->dimensions.x) * this->rat.height / this->dimensions.y,
        this->rat.width / this->dimensions.x,
        this->rat.height / this->dimensions.y
    );
}

void Rat::addAnimation(char const *name, Rat::Animation animation) {
    this->animations[name] = animation;
}

void Rat::play(char const *name) {
    if (this->animations.count(name) == 1) {
        this->current = this->animations.at(name);
        this->rolling = true;
        this->timer = 0;
    } else {
        spdlog::error("Couldn't find animation '{}'", name);
    }
}

void Rat::stop() {
    this->rolling = false;

}

int Rat::isPlaying() const {
    if (!this->current) {
        return false;
    } else if (this->current->loop) {
        return true;
    } else {
        return this->timer < this->current->frames.size() *
            this->current->frameTime;
    }
}
