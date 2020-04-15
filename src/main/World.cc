#include "World.hh"
#include "Const.hh"

World::World(
    sf::Texture const *ground,
    sf::Color horizon,
    sf::Color bottomSky,
    sf::Color topSky
): background(sf::IntRect(0, 0, Const::WIDTH, Const::HEIGHT)) {
    this->background.initFromString(World::BACKGROUND_SHADER);
    this->background.setTexture(ground);
    this->background.setUniform("horizon", horizon);
    this->background.setUniform("bottomSky", bottomSky);
    this->background.setUniform("topSky", topSky);
}

std::pair<char const *, char const *> World::update(sf::Vector2f camera) {
    this->background.update();
    char const *function = NULL;
    char const *argument = NULL;
    // Update the lindels.
    for (Lindel &lindel: this->lindels) {
        lindel.update();
        // TODO: collisions and that kind of thing maybe.
    }
    // Update the player.
    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;
    this->position.z += this->velocity.z;
    this->velocity.x += this->gravity.x;
    this->velocity.y += this->gravity.y;
    this->velocity.z += this->gravity.z;
    // Return something.
    return std::pair(function, argument);
}

void World::draw(
    sf::RenderTarget &target,
    Renderer &renderer,
    sf::Vector2f camera
) const {
    this->background.draw(target);
    // Draw the lindels.
    sf::Vector3f floor = Util::sphereToScreen(
        sf::Vector2f(0, Const::HALF_PI * ((camera.y > 0) ? 1 : -1)),
        camera
    );
    sf::Vector2f floorScreen(floor.x, floor.y);
    for (Lindel const &lindel: this->lindels) {
        if (!lindel.alive) continue;
        sf::Vector3f spherePos = Util::toSphere(
            lindel.position,
            this->position
        );
        sf::Vector3f screenPos = Util::sphereToScreen(sf::Vector2f(
            spherePos.x,
            spherePos.y
        ), camera);
        if (screenPos.z < 0) continue;
        float angle = Util::upAngle(
            camera,
            floorScreen,
            sf::Vector2f(screenPos.x, screenPos.y)
        );
        float scale = 1 / screenPos.z;
        renderer.batch.draw(
            lindel.entity.sprite,
            sf::Vector2f(screenPos.x, screenPos.y),
            lindel.entity.offset,
            angle,
            sf::Vector2f(scale, scale)
        );
    }
}

void World::addLindel(Entity const &entity, sf::Vector3f position) {
    Lindel lindel(entity);
    lindel.position = position;
    this->lindels.push_back(lindel);
}
