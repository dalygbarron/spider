#include "Renderer.hh"
#include "spdlog/spdlog.h"

Renderer::Renderer(sf::Texture const &sprites, glm::ivec2 size):
    batch(sprites, size)
{
    // does nothing else.
}

void Renderer::setCursorRat(Rectangle rat, Renderer::CursorType type) {
    this->cursorRats[static_cast<int>(type)] = rat;
}

void Renderer::point(glm::vec2 pos, int highlight) const {
    this->batch.draw(
        highlight ? this->pointHighlightRat : this->pointRat,
        pos
    );
}

void Renderer::node(glm::vec2 pos, int highlight) const {
    this->batch.draw(highlight ? this->nodeHighlightRat : this->nodeRat, pos);
}

void Renderer::cursor(glm::vec2 pos, Renderer::CursorType cursor) const {
    this->batch.draw(this->cursorRats[static_cast<int>(cursor)], pos);
}

void Renderer::box(Rectangle pos, int highlight) const {
    this->batch.draw(
        highlight ? this->boxHighlightPatch : this->boxPatch,
        pos
    );
}

void Renderer::panel(Rectangle pos) const {
    this->batch.draw(this->panelPatch, pos);
}

void Renderer::button(Rectangle pos, int depressed) const {
    this->batch.draw(
        depressed ? this->buttonDepressedPatch : this->buttonPatch,
        pos
    );
}

void Renderer::line(
    glm::vec2 start,
    glm::vec2 end,
    int highlight
) const {
    this->batch.draw(
        highlight ? this->lineHighlightRat : this->lineRat,
        start,
        end
    );
}

void Renderer::club(
    glm::vec2 start,
    glm::vec2 end,
    int highlight
) const {
    this->line(start, end, highlight);
    this->node(start, highlight);
}

void Renderer::sphereMesh(
    Mesh const &mesh,
    glm::vec2 camera,
    int highlight
) const {
    std::vector<glm::vec2> const &vertices = mesh.getVertices();
    int n = vertices.size();
    for (int i = 0; i < n; i++) {
        int next = (i == n - 1) ? 0 : i + 1;
        glm::vec2 delta = vertices[next] - vertices[i];
        delta.x /= Renderer::SPHERE_INTERPOLATION;
        delta.y /= Renderer::SPHERE_INTERPOLATION;
        for (int j = 0; j < Renderer::SPHERE_INTERPOLATION; j++) {
            glm::vec2 multipliedDelta = glm::vec2(
                delta.x * j,
                delta.y * j
            );
            sf::Vector3f start = sf::Vector3f(0, 0, 0);
            sf::Vector3f end = sf::Vector3f(0, 0, 0);
            if (start.z < 0) continue;
            this->line(
                glm::vec2(start.x, start.y),
                glm::vec2(end.x, end.y),
                i == highlight
            );
        }
        sf::Vector3f nodePos = sf::Vector3f(0, 0, 0);
        if (nodePos.z < 0) continue;
        this->node(glm::vec2(nodePos.x, nodePos.y), i == highlight);
    }
}

void Renderer::arc(glm::vec2 pos, float radius, float a, float b) const {
    this->batch.draw(this->lineRat, pos, radius, a, b);
}

void Renderer::text(char const *content, glm::vec2 pos) const {
    this->text(content, pos, this->font);
}

void Renderer::text(
    char const *content,
    glm::vec2 pos,
    Rectangle font
) const {
    float originX = pos.x;
    glm::ivec2 character = font.size / 16;
    for (int i = 0; content[i]; i++) {
        char c = content[i];
        if (c == '\n') {
            pos.x = originX;
            pos.y += character.y;
            continue;
        }
        this->batch.draw(
            Rectangle(
                font.pos + character * glm::ivec2(c % 16, c / 16),
                character
            ),
            Rectangle(pos, character)
        );
        pos.x += character.x - 1;
    }
}

void Renderer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(this->batch);
}
