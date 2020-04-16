#include "World.hh"
#include "Const.hh"
#include "spdlog/spdlog.h"

static char const *BACKGROUND_SHADER = R"~~~(
    #ifdef GL_ES
    precision mediump float;
    #endif

    #define HALF_PI 1.57079632679

    uniform sampler2D texture;
    uniform vec2 offset;
    uniform vec2 resolution;
    uniform int time;
    uniform vec4 horizon;
    uniform vec4 bottomSky;
    uniform vec4 topSky;
    uniform vec3 position;
    uniform vec2 rotation;

    vec2 fov = vec2(2.094395102, 1.570796327);

    void main() {
        vec2 uv = gl_FragCoord.xy / resolution - vec2(0.5, 0.5);
        uv.y = 1.0 - uv.y;
        vec2 angle = rotation + uv * fov;
        vec4 colour = vec4(1.0);
        if (angle.y > HALF_PI) {
            float distance = tan(angle.y) * position.z;
            vec2 texturePoint = vec2(
                cos(angle.x) * distance,
                sin(angle.x) * distance
            );
            colour = texture2D(texture, position.xy + texturePoint);
        } else {
            colour = mix(bottomSky, topSky, uv.y + 0.5);
        }
        float haze = 1.0 - abs(angle.y - HALF_PI);
        gl_FragColor = colour + horizon * haze / 2.0;
    })~~~";

World::World(
    sf::Texture const *ground,
    sf::Color horizon,
    sf::Color bottomSky,
    sf::Color topSky
): background(sf::IntRect(0, 0, Const::WIDTH, Const::HEIGHT)) {
    this->background.initFromString(BACKGROUND_SHADER);
    this->background.setTexture(ground);
    this->background.setUniform("horizon", horizon);
    this->background.setUniform("bottomSky", bottomSky);
    this->background.setUniform("topSky", topSky);
    this->position.z = 1;
    this->velocity.y = -0.02;
}

std::pair<char const *, char const *> World::update(sf::Vector2f camera) {
    this->background.setUniform("rotation", camera);
    this->background.setUniform("position", this->position);
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
            this->position,
            lindel.position
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
        float scale = 2 / spherePos.z;
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
    lindel.alive = true;
    this->lindels.push_back(lindel);
}
