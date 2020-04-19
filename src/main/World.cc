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
    this->velocity.y = -0.03;
    this->gravity.z = -0.02;
}

std::pair<char const *, char const *> World::update(Matrix const &camera) {
    // TODO: make camera and position into one 4x4 matrix
    this->background.setUniform("camera", camera);
    this->background.setUniform("position", this->position);
    this->background.update();
    char const *function = NULL;
    char const *argument = NULL;
    // Update the lindels.
    for (Lindel &lindel: this->lindels) {
        lindel.update();
        // TODO: collisions and that kind of thing maybe.
    }
    if (this->position.z < 0.5) this->velocity.z = 0.3;
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
    Matrix const &camera
) const {
    this->background.draw(target);
    // Draw the lindels.
    sf::Vector3f floor = Util::transformPoint(
        sf::Vector3f(0, -1, 0),
        camera
    );
    // TODO: floorscreen is wrong I think.
    sf::Vector2f floorScreen(floor.x, floor.y);
    for (Lindel const &lindel: this->lindels) {
        if (!lindel.alive) continue;
        sf::Vector3f pos = Util::transformPoint(lindel.position, camera);
        sf::Vector2f screenPos(pos.x, pos.y);
        // TODO: still need to project camera coordinates toscreen.
        float angle = Util::upAngle(
            floorScreen,
            floorScreen,
            sf::Vector2f(screenPos.x, screenPos.y)
        );
        float scale = 1;
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
