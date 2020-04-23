#include "RatBatch.hh"
#include "Util.hh"
#include "Const.hh"
#include "spdlog/spdlog.h"

RatBatch::RatBatch(sf::Texture const &texture): texture(texture) {
    this->clear();
}

void RatBatch::clear() {
    this->n = 0;
    this->clean = true;
}

void RatBatch::draw(sf::IntRect sprite, glm::vec2 pos) {
    sf::FloatRect box;
    box.left = pos.x - sprite.width / 2;
    box.top = pos.y - sprite.height / 2;
    box.width = sprite.width;
    box.height = sprite.height;
    this->draw(sprite, box);
}

void RatBatch::draw(
    sf::IntRect sprite,
    glm::vec2 pos,
    glm::vec2 offset,
    float rot,
    glm::vec2 scale
) {
    sf::Transform transform;
    transform.translate(pos)
        .rotate(Util::degrees(rot))
        .scale(scale)
        .translate(offset);
    int nVertices = this->n * 4;
    this->reserve(nVertices + 4);
    this->vertices[nVertices].position = transform.transformPoint(
        -sprite.width / 2,
        -sprite.height / 2
    );
    this->vertices[nVertices].texCoords = glm::vec2(sprite.left, sprite.top);
    this->vertices[nVertices + 1].position = transform.transformPoint(
        sprite.width / 2,
        -sprite.height / 2
    );
    this->vertices[nVertices + 1].texCoords = glm::vec2(
        sprite.left + sprite.width,
        sprite.top
    );
    this->vertices[nVertices + 2].position = transform.transformPoint(
        sprite.width / 2,
        sprite.height / 2
    );
    this->vertices[nVertices + 2].texCoords = glm::vec2(
        sprite.left + sprite.width,
        sprite.top + sprite.height
    );
    this->vertices[nVertices + 3].position = transform.transformPoint(
        -sprite.width / 2,
        sprite.height / 2
    );
    this->vertices[nVertices + 3].texCoords = glm::vec2(
        sprite.left,
        sprite.top + sprite.height
    );
    this->n++;
}

void RatBatch::draw(Patch const &patch, sf::FloatRect pos) {
    this->draw(patch.topLeft, sf::FloatRect(
        pos.left,
        pos.top,
        patch.topLeft.width,
        patch.topLeft.height
    ));
    this->draw(patch.top, sf::FloatRect(
        pos.left + patch.topLeft.width,
        pos.top,
        pos.width - (patch.topLeft.width + patch.topRight.width),
        patch.top.height
    ));
    this->draw(patch.topRight, sf::FloatRect(
        pos.left + pos.width - patch.topRight.width,
        pos.top,
        patch.topRight.width,
        patch.topRight.height
    ));
    this->draw(patch.right, sf::FloatRect(
        pos.left + pos.width - patch.right.width,
        pos.top + patch.topRight.height,
        patch.right.width,
        pos.height - (patch.topRight.height + patch.topLeft.height)
    ));
    this->draw(patch.bottomRight, sf::FloatRect(
        pos.left + pos.width - patch.right.width,
        pos.top + pos.height - patch.bottomRight.height,
        patch.bottomRight.width,
        patch.bottomRight.height
    ));
    this->draw(patch.bottom, sf::FloatRect(
        pos.left + patch.bottomLeft.width,
        pos.top + pos.height - patch.bottom.height,
        pos.width - (patch.bottomLeft.width + patch.bottomRight.width),
        patch.bottom.height
    ));
    this->draw(patch.bottomLeft, sf::FloatRect(
        pos.left,
        pos.top + pos.height - patch.bottomLeft.height,
        patch.bottomLeft.width,
        patch.bottomLeft.height
    ));
    this->draw(patch.left, sf::FloatRect(
        pos.left,
        pos.top + patch.topLeft.height,
        patch.left.width,
        pos.height - (patch.topLeft.height + patch.bottomLeft.height)
    ));
    this->draw(patch.middle, sf::FloatRect(
        pos.left + patch.left.width,
        pos.top + patch.top.height,
        pos.width - (patch.left.width + patch.right.width),
        pos.height - (patch.top.height + patch.bottom.height)
    ));
}

void RatBatch::draw(sf::IntRect sprite, sf::FloatRect pos) {
    int nVertices = this->n * 4;
    this->reserve(nVertices + 4);
    this->vertices[nVertices].position = glm::vec2(pos.left, pos.top);
    this->vertices[nVertices].texCoords = glm::vec2(sprite.left, sprite.top);
    this->vertices[nVertices + 1].position = glm::vec2(
        pos.left + pos.width,
        pos.top
    );
    this->vertices[nVertices + 1].texCoords = glm::vec2(
        sprite.left + sprite.width,
        sprite.top
    );
    this->vertices[nVertices + 2].position = glm::vec2(
        pos.left + pos.width,
        pos.top + pos.height
    );
    this->vertices[nVertices + 2].texCoords = glm::vec2(
        sprite.left + sprite.width,
        sprite.top + sprite.height
    );
    this->vertices[nVertices + 3].position = glm::vec2(
        pos.left,
        pos.top + pos.height
    );
    this->vertices[nVertices + 3].texCoords = glm::vec2(
        sprite.left,
        sprite.top + sprite.height
    );
    this->n++;
}

void RatBatch::draw(sf::IntRect sprite, glm::vec2 start, glm::vec2 end) {
    // Figure out what orientation to draw them with.
    glm::vec2 edge(0, 0);
    if (fabs(end.x - start.x) > fabs(end.y - start.y)) {
        edge.y = sprite.height / 2;
    } else {
        edge.x = sprite.width / 2;
    }
    int nVertices = this->n * 4;
    if (nVertices + 4 > this->vertices.size()) {
        this->vertices.resize(this->vertices.size() + 4);
    }
    this->vertices[nVertices].position = glm::vec2(
        start.x - edge.x,
        start.y - edge.y
    );
    this->vertices[nVertices].texCoords = glm::vec2(
        sprite.left,
        sprite.top + sprite.height
    );
    this->vertices[nVertices + 1].position = glm::vec2(
        end.x - edge.x,
        end.y - edge.y
    );
    this->vertices[nVertices + 1].texCoords = glm::vec2(
        sprite.left,
        sprite.top
    );
    this->vertices[nVertices + 2].position = glm::vec2(
        end.x + edge.x,
        end.y + edge.y
    );
    this->vertices[nVertices + 2].texCoords = glm::vec2(
        sprite.left + sprite.width,
        sprite.top
    );
    this->vertices[nVertices + 3].position = glm::vec2(
        start.x + edge.x,
        start.y + edge.y
    );
    this->vertices[nVertices + 3].texCoords = glm::vec2(
        sprite.left + sprite.width,
        sprite.top + sprite.height
    );
    this->n++;
}

void RatBatch::draw(
    sf::IntRect sprite,
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
        this->vertices[nVertices + i * 4].position = glm::vec2(
            pos.x + cos(a + delta * i) * radius,
            pos.y + sin(a + delta * i) * radius
        );
        this->vertices[nVertices + i * 4].texCoords = glm::vec2(
            sprite.left + sprite.width,
            sprite.top + sprite.height
        );
        this->vertices[nVertices + i * 4 + 1].position = glm::vec2(
            pos.x + cos(a + delta * i + delta) * radius,
            pos.y + sin(a + delta * i + delta) * radius
        );
        this->vertices[nVertices + i * 4 + 1].texCoords = glm::vec2(
            sprite.left + sprite.width,
            sprite.top
        );
        this->vertices[nVertices + i * 4 + 2].position = glm::vec2(
            pos.x + cos(a + delta * i + delta) * (radius - sprite.width),
            pos.y + sin(a + delta * i + delta) * (radius - sprite.width)
        );
        this->vertices[nVertices + i * 4 + 2].texCoords = glm::vec2(
            sprite.left,
            sprite.top + sprite.height
        );
        this->vertices[nVertices + i * 4 + 3].position = glm::vec2(
            pos.x + cos(a + delta * i) * (radius - sprite.width),
            pos.y + sin(a + delta * i) * (radius - sprite.width)
        );
        this->vertices[nVertices + i * 4 + 3].texCoords = glm::vec2(
            sprite.left,
            sprite.top
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
