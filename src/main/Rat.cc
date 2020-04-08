#include "Rat.hh"
#include "spdlog/spdlog.h"

Rat::Rat(sf::IntRect rat, sf::Vector2u dimensions) {
    this->rat = rat;
    this->dimensions = dimensions;
}

void Rat::update() {
    if (this->rolling) this->timer++;
}

sf::Vector2u Rat::getSize() const {
    return sf::Vector2u(
        this->rat.width / this->dimensions.x,
        this->rat.height / this->dimensions.y
    );
}

sf::IntRect Rat::getFrame() const {
    int index;
    if (!this->current) {
        index = (this->timer / Rat::DEFAULT_SPEED) %
            (this->dimensions.x * this->dimensions.y);
    } else if (this->current->loop) {
        spdlog::error("{}", this->current->frames.size());
        index = (this->timer / this->current->frameTime) %
            this->current->frames.size();
    } else {
        index = fmin(
            this->timer / this->current->frameTime,
            this->current->frames.size()
        );
    }
    return sf::IntRect(
        this->rat.left + (index % this->dimensions.x) *
            this->rat.width / this->dimensions.x,
        this->rat.top + (index / this->dimensions.x) *
            this->rat.height / this->dimensions.y,
        this->rat.width / this->dimensions.x,
        this->rat.height / this->dimensions.y
    );
}

Rat::Animation const *Rat::getCurrent() const {
    return this->current;
}

void Rat::play(Rat::Animation const *animation, int priority) {
    if ((priority >= this->priority && animation != this->current) ||
        !this->isPlaying()
    ) {
        this->current = animation;
        this->rolling = true;
        this->timer = 0;
        this->priority = priority;
    }
}

void Rat::stop() {
    this->rolling = false;
    this->priority = INT_MIN;
}

int Rat::isPlaying() const {
    if (!this->current) {
        return true;
    } else if (!this->rolling) {
        return false;
    } else if (this->current->loop) {
        return true;
    } else {
        return this->timer < this->current->frames.size() *
            this->current->frameTime;
    }
}
