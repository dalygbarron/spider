#include "Renderer.hh"
#include "spdlog/spdlog.h"

Renderer::Renderer(sf::Texture const &sprites): batch(sprites) {
    // does nothing else.
}

void Renderer::setCursorRat(sf::IntRect rat, Renderer::CursorType type) {
    this->cursorRats[static_cast<int>(type)] = rat;
}

void Renderer::point(sf::Vector2f pos, int highlight) const {
    this->batch.draw(
        highlight ? this->pointHighlightRat : this->pointRat,
        pos
    );
}

void Renderer::node(sf::Vector2f pos, int highlight) const {
    this->batch.draw(highlight ? this->nodeHighlightRat : this->nodeRat, pos);
}

void Renderer::cursor(sf::Vector2f pos, Renderer::CursorType cursor) const {
    this->batch.draw(this->cursorRats[static_cast<int>(cursor)], pos);
}

void Renderer::box(sf::FloatRect pos, int highlight) const {
    this->batch.draw(
        highlight ? this->boxHighlightPatch : this->boxPatch,
        pos
    );
}

void Renderer::panel(sf::FloatRect pos) const {
    this->batch.draw(this->panelPatch, pos);
}

void Renderer::button(sf::FloatRect pos, int depressed) const {
    this->batch.draw(
        depressed ? this->buttonDepressedPatch : this->buttonPatch,
        pos
    );
}

void Renderer::line(
    sf::Vector2f start,
    sf::Vector2f end,
    int highlight
) const {
    this->batch.draw(
        highlight ? this->lineHighlightRat : this->lineRat,
        start,
        end
    );
}

void Renderer::club(
    sf::Vector2f start,
    sf::Vector2f end,
    int highlight
) const {
    this->line(start, end, highlight);
    this->node(start, highlight);
}

void Renderer::sphereMesh(
    Mesh const &mesh,
    sf::Vector2f camera,
    int highlight
) const {
    std::vector<sf::Vector2f> const &vertices = mesh.getVertices();
    int n = vertices.size();
    for (int i = 0; i < n; i++) {
        int next = (i == n - 1) ? 0 : i + 1;
        sf::Vector2f delta = vertices[next] - vertices[i];
        delta.x /= Renderer::SPHERE_INTERPOLATION;
        delta.y /= Renderer::SPHERE_INTERPOLATION;
        for (int j = 0; j < Renderer::SPHERE_INTERPOLATION; j++) {
            sf::Vector2f multipliedDelta = sf::Vector2f(
                delta.x * j,
                delta.y * j
            );
            sf::Vector3f start = Util::sphereToScreen(
                vertices[i] + multipliedDelta,
                camera
            );
            sf::Vector3f end = Util::sphereToScreen(
                vertices[i] + multipliedDelta + delta,
                camera
            );
            if (start.z < 0) continue;
            this->line(
                sf::Vector2f(start.x, start.y),
                sf::Vector2f(end.x, end.y),
                i == highlight
            );
        }
        sf::Vector3f nodePos = Util::sphereToScreen(vertices[i], camera);
        if (nodePos.z < 0) continue;
        this->node(sf::Vector2f(nodePos.x, nodePos.y), i == highlight);
    }
}

void Renderer::arc(sf::Vector2f pos, float radius, float a, float b) const {
    this->batch.draw(this->lineRat, pos, radius, a, b);
}

void Renderer::text(char const *content, sf::Vector2f pos) const {
    this->text(content, pos, this->font);
}

void Renderer::text(
    char const *content,
    sf::Vector2f pos,
    sf::IntRect font
) const {
    float originX = pos.x;
    int characterWidth = font.width / 16;
    int characterHeight = font.height / 16;
    for (int i = 0; content[i]; i++) {
        char c = content[i];
        if (c == '\n') {
            pos.x = originX;
            pos.y += characterHeight;
            continue;
        }
        this->batch.draw(
            sf::IntRect(
                font.left + (c % 16) * characterWidth,
                font.top + (c / 16) * characterHeight,
                characterWidth,
                characterHeight
            ),
            sf::FloatRect(
                pos.x,
                pos.y,
                characterWidth,
                characterHeight
            )
        );
        pos.x += characterWidth - 1;
    }
}

void Renderer::rat(Rat const &rat, sf::Vector2f pos) {
    this->batch.draw(rat.getFrame(), pos);
}

void Renderer::rat(
    Rat const &rat,
    sf::Vector2f position,
    float rotation,
    sf::Vector2f scale,
    int flip
) {
    if (flip) scale.x *= -1;
    this->batch.draw(
        rat.getFrame(),
        position,
        sf::Vector2f(0, 0),
        rotation,
        scale
    );
}

void Renderer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(this->batch);
}
