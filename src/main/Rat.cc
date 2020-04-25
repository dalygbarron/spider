#include "Rat.hh"
#include "spdlog/spdlog.h"

Rat::Rat(Rectangle rat, glm::uvec2 dimensions) {
    this->rat = rat;
    this->dimensions = dimensions;
}

void Rat::update() {
    if (this->rolling) this->timer++;
}

glm::uvec2 Rat::getSize() const {
    return (glm::uvec2)this->rat.size / this->dimensions;
}

Rectangle Rat::getFrame() const {
    int index = 0;
    if (!this->current) {
        index = (this->timer / Rat::DEFAULT_SPEED) %
            (this->dimensions.x * this->dimensions.y);
    } else if (this->current->loop && this->current->frameTime) {
        index = this->current->frames[(this->timer / this->current->frameTime) %
            this->current->frames.size()];
    } else if (this->current->frameTime) {
        index = this->current->frames[fmin(
            this->timer / this->current->frameTime,
            this->current->frames.size()
        )];
    }
    return Rectangle(
        this->rat.pos +
            glm::ivec2(index % dimensions.x, index / dimensions.y) *
            this->rat.size / (glm::ivec2)this->dimensions,
        this->rat.size / (glm::ivec2)this->dimensions
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
    if (!this->rolling) {
        return false;
    } else if (!this->current) {
        return true;
    } else if (this->current->loop) {
        return true;
    } else {
        return this->timer < this->current->frames.size() *
            this->current->frameTime;
    }
}
