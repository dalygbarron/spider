#include "Renderer.hh"
#include "spdlog/spdlog.h"

Renderer::Renderer(sf::Texture const &sprites, glm::ivec2 size):
    batch(sprites, size),
    size(size)
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
    glm::mat4 camera,
    int highlight
) const {
    std::vector<glm::vec2> const &vertices = mesh.getVertices();
    int n = vertices.size();
    for (int i = 0; i < n; i++) {
        int next = (i == n - 1) ? 0 : i + 1;
        glm::vec2 delta = (vertices[next] - vertices[i]) /
            (float)Renderer::SPHERE_INTERPOLATION;
        for (int j = 0; j < Renderer::SPHERE_INTERPOLATION; j++) {
            glm::vec3 start = Util::sphericalToCartesian(
                vertices[i] + delta * (float)j
            );
            glm::vec3 end = Util::sphericalToCartesian(
                vertices[i] + delta * (j + 1.0f)
            );
            glm::vec4 startP = camera * glm::vec4(start, 0);
            glm::vec4 endP = camera * glm::vec4(end, 0);
            if (startP.w < 0 || endP.w < 0) continue;
            startP = startP / startP.w;
            endP = endP / endP.w;
            glm::vec2 startScreen = glm::vec2(1 - startP.x, 1 - startP.y) *
                0.5f * (glm::vec2)this->size;
            glm::vec2 endScreen = glm::vec2(1 - endP.x, 1 - endP.y) *
                0.5f * (glm::vec2)this->size;
            this->line(startScreen, endScreen, i == highlight);
        }
        glm::vec3 nodePos = Util::sphericalToCartesian(vertices[i]);
        glm::vec4 nodeP = camera * glm::vec4(nodePos, 0);
        if (nodeP.w < 0) continue;
        nodeP = nodeP / nodeP.w;
        glm::vec2 nodeScreen = glm::vec2(1 - nodeP.x, 1 - nodeP.y) * 0.5f *
            (glm::vec2)this->size;
        this->node(nodeScreen, i == highlight);
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
