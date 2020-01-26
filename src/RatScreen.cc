#include "Screen.hh"
#include "Const.hh"

void RatScreen::Rat::update(float delta) {
    this->rotation += this->angularVelocity * delta;
    this->position += this->velocity * delta;
    this->velocity.x += this->gravity.x * delta;
    this->velocity.y += this->gravity.y * delta;
    if (this->position.x < 0) this->position.x += Const::WIDTH;
    if (this->position.x >= Const::WIDTH) this->position.x -= Const::WIDTH;
    if (this->position.y < 0) this->position.y += Const::HEIGHT;
    if (this->position.y >= Const::HEIGHT) this->position.y -= Const::HEIGHT;
}

RatScreen::RatScreen(Core &core): Screen(core) {
    this->rats.resize(this->core.spritesheet.count() * 15);
    int i = 0;
    for (std::unordered_map<std::string, sf::IntRect>::const_iterator it =
        this->core.spritesheet.begin();
        it != this->core.spritesheet.end();
        it++
    ) {
        for (int j = 0; j < 15; j++) {
            sf::IntRect sprite = it->second;
            this->rats[i].sprite.left = sprite.left;
            this->rats[i].sprite.top = sprite.top;
            this->rats[i].sprite.width = sprite.width;
            this->rats[i].sprite.height = sprite.height;
            float moveAngle = fmod(rand(), Const::DOUBLE_PI);
            float gravityAngle = fmod(rand(), Const::DOUBLE_PI);
            float rotationAngle = fmod(rand(), Const::DOUBLE_PI);
            this->rats[i].velocity.x = 55 * cos(moveAngle);
            this->rats[i].velocity.y = 55 * sin(moveAngle);
            this->rats[i].gravity.x = 3 * cos(gravityAngle);
            this->rats[i].gravity.y = 3 * sin(gravityAngle);
            this->rats[i].rotation = rotationAngle;
            this->rats[i].angularVelocity = (float)(rand() % 30) / 5 - 3;
            i++;
        }
    }
}

Screen *RatScreen::update(float delta, sf::Window &window) {
    for (int i = 0; i < this->rats.size(); i++) {
        this->rats[i].update(delta);
    }
    return NULL;
}

void RatScreen::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.clear(sf::Color(252, 194, 3));
    this->core.batch.clear();
    for (int i = 0; i < this->rats.size(); i++) {
        this->core.batch.draw(
            this->rats[i].sprite,
            this->rats[i].position,
            this->rats[i].rotation
        );
    }
    target.draw(this->core.batch);
}
