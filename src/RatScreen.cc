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
    spdlog::info(
        "Creating rat screen with {} rats",
        this->core.spritesheet.count()
    );
    this->rats.resize(this->core.spritesheet.count());
    int i = 0;
    for (std::unordered_map<std::string, sf::IntRect>::const_iterator it =
        this->core.spritesheet.begin();
        it != this->core.spritesheet.end();
        it++
    ) {
        sf::IntRect sprite = it->second;
        this->rats[i].sprite.left = sprite.left;
        this->rats[i].sprite.top = sprite.top;
        this->rats[i].sprite.width = sprite.width;
        this->rats[i].sprite.height = sprite.height;
        float moveAngle = fmod((float)rand() / 100, Const::DOUBLE_PI);
        float gravityAngle = fmod((float)rand() / 100, Const::DOUBLE_PI);
        float rotationAngle = fmod((float)rand() / 100, Const::DOUBLE_PI);
        float scale = (float)(rand() % 100) / 80;
        this->rats[i].velocity.x = 65 * scale * cos(moveAngle);
        this->rats[i].velocity.y = 65 * scale * sin(moveAngle);
        this->rats[i].gravity.x = 12 * scale * cos(gravityAngle);
        this->rats[i].gravity.y = 12 * scale * sin(gravityAngle);
        this->rats[i].rotation = rotationAngle;
        this->rats[i].angularVelocity = (float)(rand() % 30) / 5 - 3;
        this->rats[i].scale = sf::Vector2f(scale, scale);
        i++;
    }
    std::sort(
        this->rats.begin(),
        this->rats.end(),
        [](Rat const &a, Rat const &b) -> int {
            return a.scale.x < b.scale.x;
        }
    );
}

Screen *RatScreen::update(float delta, sf::RenderWindow &window) {
    for (int i = 0; i < this->rats.size(); i++) {
        this->rats[i].update(delta);
    }
    return NULL;
}

void RatScreen::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.clear(sf::Color::Cyan);
    this->core.renderer.batch.clear();
    for (int i = 0; i < this->rats.size(); i++) {
        this->core.renderer.batch.draw(
            this->rats[i].sprite,
            this->rats[i].position,
            this->rats[i].rotation,
            this->rats[i].scale
        );
    this->core.renderer.point(this->rats[i].position);
    sf::FloatRect box(
        this->rats[i].position.x - this->rats[i].sprite.width / 2,
        this->rats[i].position.y - this->rats[i].sprite.height / 2,
        this->rats[i].sprite.width,
        this->rats[i].sprite.height
    );
    this->core.renderer.box(box, false);
    }
    target.draw(this->core.renderer);
}
