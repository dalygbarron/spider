#include "World.hh"
#include "Const.hh"
#include "Shaders.hh"
#include "spdlog/spdlog.h"

static GLfloat const vertexData[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.5f, 1.0f, 0.0f
};

World::World() {
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    glGenBuffers(1, &this->skyVB);
    glBindBuffer(GL_ARRAY_BUFFER, this->skyVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    this->skyShader = Util::loadShader(
        Shaders::VERTEX_SHADER,
        Shaders::COLOUR_SHADER
    );
}

World::~World() {
    glDeleteBuffers(1, &this->skyVB);
}

void World::setShell(sf::Texture const *texture) {
    // TODO: stuff.
}

std::pair<char const *, char const *> World::update(glm::mat4 const &c) {
    // TODO: make camera and position into one 4x4 matrix
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
    glm::mat4 const &c
) const {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->skyVB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glUseProgram(this->skyShader);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

void World::addLindel(Entity const &entity, sf::Vector3f position) {
    Lindel lindel(entity);
    lindel.position = position;
    lindel.alive = true;
    this->lindels.push_back(lindel);
}
