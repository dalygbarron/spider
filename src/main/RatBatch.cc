#include "RatBatch.hh"
#include "Util.hh"
#include "Const.hh"
#include "spdlog/spdlog.h"
#include "glm/gtc/matrix_transform.hpp"

RatBatch::RatBatch(sf::Texture const &texture): texture(texture) {
    this->clear();
}

void RatBatch::clear() {
    this->n = 0;
    this->clean = true;
}

void RatBatch::draw(Rectangle sprite, glm::vec2 pos) {
    this->draw(
        sprite,
        Rectangle(pos - (glm::vec2)sprite.size * glm::vec2(0.5, 0.5), sprite.size)
    );
}

void RatBatch::draw(
    Rectangle sprite,
    glm::vec2 pos,
    glm::vec2 offset,
    float rot,
    glm::vec2 scale
) {
    glm::mat4 transform(1);
    transform = glm::translate(transform, glm::vec3(pos.x - offset.x, pos.y - offset.y, 0));
    transform = glm::rotate(transform, rot, glm::vec3(0, 0, -1));
    transform = glm::scale(transform, glm::vec3(scale.x, scale.y, 1));
    glm::vec4 topLeft = transform * glm::vec4(-sprite.size.x / 2, -sprite.size.y / 2, 0, 1);
    glm::vec4 topRight = transform * glm::vec4(sprite.size.x / 2, -sprite.size.y / 2, 0, 1);
    glm::vec4 bottomRight = transform * glm::vec4(sprite.size.x / 2, sprite.size.y / 2, 0, 1);
    glm::vec4 bottomLeft = transform * glm::vec4(-sprite.size.x / 2, sprite.size.y / 2, 0, 1);
    int nVertices = this->n * 4;
    this->reserve(nVertices + 4);
    this->vertices[nVertices].position = sf::Vector2f(topLeft.x, topLeft.y);
    this->vertices[nVertices].texCoords = sf::Vector2f(
        sprite.pos.x,
        sprite.pos.y
    );
    this->vertices[nVertices + 1].position = sf::Vector2f(
        topRight.x,
        topRight.y
    );
    this->vertices[nVertices + 1].texCoords = sf::Vector2f(
        sprite.pos.x + sprite.size.x,
        sprite.pos.y
    );
    this->vertices[nVertices + 2].position = sf::Vector2f(
        bottomRight.x,
        bottomRight.y
    );
    this->vertices[nVertices + 2].texCoords = sf::Vector2f(
        sprite.pos.x + sprite.size.x,
        sprite.pos.y + sprite.size.y
    );
    this->vertices[nVertices + 3].position = sf::Vector2f(
        bottomLeft.x,
        bottomLeft.y
    );
    this->vertices[nVertices + 3].texCoords = sf::Vector2f(
        sprite.pos.x,
        sprite.pos.y + sprite.size.y
    );
    this->n++;
}

void RatBatch::draw(Patch const &p, Rectangle pos) {
    this->draw(p.topLeft, Rectangle(pos.pos, p.topLeft.size));
    this->draw(p.top, Rectangle(
        pos.pos + glm::ivec2(p.topLeft.size.x, 0),
        glm::vec2(pos.size.x - (p.topLeft.size.x + p.topRight.size.x), p.top.size.y)
    ));
    this->draw(p.topRight, Rectangle(
        pos.pos + glm::ivec2(pos.size.x - p.topRight.size.x, 0),
        p.topRight.size
    ));
    this->draw(p.right, Rectangle(
        pos.pos + glm::ivec2(pos.size.x - p.right.size.x, p.topRight.size.y),
        glm::ivec2(
            p.right.size.x,
            pos.size.y - (p.topRight.size.y + p.topLeft.size.y)
        )
    ));
    this->draw(p.bottomRight, Rectangle(
        pos.pos + pos.size - glm::ivec2(p.right.size.x, p.bottomRight.size.y),
        p.bottomRight.size
    ));
    this->draw(p.bottom, Rectangle(
        pos.pos + glm::ivec2(p.bottomLeft.size.x, pos.size.y - p.bottom.size.y),
        glm::ivec2(
            pos.size.x - (p.bottomLeft.size.x + p.bottomRight.size.x),
            p.bottom.size.y
        )
    ));
    this->draw(p.bottomLeft, Rectangle(
        pos.pos + glm::ivec2(0, pos.size.y - p.bottomLeft.size.y),
        p.bottomLeft.size
    ));
    this->draw(p.left, Rectangle(
        pos.pos + glm::ivec2(0, p.topLeft.size.y),
        glm::ivec2(
            p.left.size.x,
            pos.size.y - (p.topLeft.size.y + p.bottomLeft.size.y)
        )
    ));
    this->draw(p.middle, Rectangle(
        pos.pos + glm::ivec2(p.left.size.x, p.top.size.y),
        pos.size - glm::ivec2(
            p.left.size.x + p.right.size.x,
            p.top.size.y + p.bottom.size.y
        )
    ));
}

void RatBatch::draw(Rectangle sprite, Rectangle pos) {
    int nVertices = this->n * 4;
    this->reserve(nVertices + 4);
    this->vertices[nVertices].position = sf::Vector2f(pos.pos.x, pos.pos.y);
    this->vertices[nVertices].texCoords = sf::Vector2f(
        sprite.pos.x,
        sprite.pos.y
    );
    this->vertices[nVertices + 1].position = sf::Vector2f(
        pos.pos.x + pos.size.x,
        pos.pos.y
    );
    this->vertices[nVertices + 1].texCoords = sf::Vector2f(
        sprite.pos.x + sprite.size.x,
        sprite.pos.y
    );
    this->vertices[nVertices + 2].position = sf::Vector2f(
        pos.pos.x + pos.size.x,
        pos.pos.y + pos.size.y
    );
    this->vertices[nVertices + 2].texCoords = sf::Vector2f(
        sprite.pos.x + sprite.size.x,
        sprite.pos.y + sprite.size.y
    );
    this->vertices[nVertices + 3].position = sf::Vector2f(
        pos.pos.x,
        pos.pos.y + pos.size.y
    );
    this->vertices[nVertices + 3].texCoords = sf::Vector2f(
        sprite.pos.x,
        sprite.pos.y + sprite.size.y
    );
    this->n++;
}

void RatBatch::draw(Rectangle sprite, glm::vec2 start, glm::vec2 end) {
    int nVertices = this->n * 4;
    if (nVertices + 4 > this->vertices.size()) {
        this->vertices.resize(this->vertices.size() + 4);
    }
    // Figure out what orientation to draw them with.
    glm::vec2 edge(0, 0);
    if (fabs(end.x - start.x) > fabs(end.y - start.y)) {
        edge.y = sprite.size.y / 2;
    } else {
        edge.x = sprite.size.x / 2;
    }
    this->vertices[nVertices].position = sf::Vector2f(
        start.x - edge.x,
        start.y - edge.y
    );
    this->vertices[nVertices].texCoords = sf::Vector2f(
        sprite.pos.x,
        sprite.pos.y + sprite.size.y
    );
    this->vertices[nVertices + 1].position = sf::Vector2f(
        end.x - edge.x,
        end.y - edge.y
    );
    this->vertices[nVertices + 1].texCoords = sf::Vector2f(
        sprite.pos.x,
        sprite.pos.y
    );
    this->vertices[nVertices + 2].position = sf::Vector2f(
        end.x + edge.x,
        end.y + edge.y
    );
    this->vertices[nVertices + 2].texCoords = sf::Vector2f(
        sprite.pos.x + sprite.size.x,
        sprite.pos.y
    );
    this->vertices[nVertices + 3].position = sf::Vector2f(
        start.x + edge.x,
        start.y + edge.y
    );
    this->vertices[nVertices + 3].texCoords = sf::Vector2f(
        sprite.pos.x + sprite.size.x,
        sprite.pos.y + sprite.size.y
    );
    this->n++;
}

void RatBatch::draw(
    Rectangle sprite,
    glm::vec2 pos,
    float radius,
    float a,
    float b
) {
    int nVertices = this->n * 4;
    int newSize = nVertices + 4 * RatBatch::CIRCLE_SEGMENTS;
    this->reserve(newSize);
    float delta = (b - a) / RatBatch::CIRCLE_SEGMENTS;
    for (int i = 0; i < RatBatch::CIRCLE_SEGMENTS; i++) {
        this->vertices[nVertices + i * 4].position = sf::Vector2f(
            pos.x + cos(a + delta * i) * radius,
            pos.y + sin(a + delta * i) * radius
        );
        this->vertices[nVertices + i * 4].texCoords = sf::Vector2f(
            sprite.pos.x + sprite.size.x,
            sprite.pos.y + sprite.size.y
        );
        this->vertices[nVertices + i * 4 + 1].position = sf::Vector2f(
            pos.x + cos(a + delta * i + delta) * radius,
            pos.y + sin(a + delta * i + delta) * radius
        );
        this->vertices[nVertices + i * 4 + 1].texCoords = sf::Vector2f(
            sprite.pos.x + sprite.size.x,
            sprite.pos.y
        );
        this->vertices[nVertices + i * 4 + 2].position = sf::Vector2f(
            pos.x + cos(a + delta * i + delta) * (radius - sprite.size.x),
            pos.y + sin(a + delta * i + delta) * (radius - sprite.size.x)
        );
        this->vertices[nVertices + i * 4 + 2].texCoords = sf::Vector2f(
            sprite.pos.x,
            sprite.pos.y + sprite.size.y
        );
        this->vertices[nVertices + i * 4 + 3].position = sf::Vector2f(
            pos.x + cos(a + delta * i) * (radius - sprite.size.x),
            pos.y + sin(a + delta * i) * (radius - sprite.size.x)
        );
        this->vertices[nVertices + i * 4 + 3].texCoords = sf::Vector2f(
            sprite.pos.x,
            sprite.pos.y
        );
    }
    this->n += RatBatch::CIRCLE_SEGMENTS;
}

void RatBatch::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (!this->clean) {
        spdlog::critical("RatBatch not cleared between draw calls. Abort.");
        exit(1);
    }
    this->clean = false;
    states.texture = &this->texture;
    sf::Vector2u size = this->texture.getSize();
    target.draw(
        this->vertices.data(),
        this->n * 4,
        sf::PrimitiveType::Quads,
        states
    );
}

void RatBatch::reserve(int more) {
    int current = this->vertices.size();
    if (more > current) this->vertices.resize(more);
    for (int i = current; i < more; i++) {
        this->vertices[i].color = sf::Color::White;
    }
}
