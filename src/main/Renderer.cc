#include "Renderer.hh"
#include "spdlog/spdlog.h"

Renderer::Renderer(sf::Texture const &sprites): batch(sprites) {
    // does nothing else.
}

void Renderer::setPointRat(sf::IntRect rat) {
    this->pointRat = rat;
}

void Renderer::setPointHighlightRat(sf::IntRect rat) {
    this->pointHighlightRat = rat;
}

void Renderer::setLineRat(sf::IntRect rat) {
    this->lineRat = rat;
}

void Renderer::setLineHighlightRat(sf::IntRect rat) {
    this->lineHighlightRat = rat; }

void Renderer::setNodeRat(sf::IntRect rat) {
    this->nodeRat = rat;
}

void Renderer::setNodeHighlightRat(sf::IntRect rat) {
    this->nodeHighlightRat = rat;
}

void Renderer::setCursorRat(sf::IntRect rat, Renderer::CursorType cursor) {
    this->cursorRats[static_cast<int>(cursor)] = rat;
}

void Renderer::setBoxPatch(Patch patch) {
    this->boxPatch = patch;
}

void Renderer::setBoxHighlightPatch(Patch patch) {
    this->boxHighlightPatch = patch;
}

void Renderer::setPanelPatch(Patch patch) {
    this->panelPatch = patch;
}

void Renderer::setButtonPatch(Patch patch) {
    this->buttonPatch = patch;
}

void Renderer::setButtonDepressedPatch(Patch patch) {
    this->buttonDepressedPatch = patch;
}

void Renderer::setFont(Font font) {
    this->font = font;
    this->measurements.normalFontSize.x = font.bounds.width / 16;
    this->measurements.normalFontSize.y = font.bounds.height / 16;
}

Measurements const &Renderer::getMeasurements() {
    return measurements;
}

void Renderer::point(sf::Vector2f pos, int highlight) {
    this->batch.draw(
        highlight ? this->pointHighlightRat : this->pointRat,
        pos
    );
}

void Renderer::node(sf::Vector2f pos, int highlight) {
    this->batch.draw(highlight ? this->nodeHighlightRat : this->nodeRat, pos);
}

void Renderer::cursor(sf::Vector2f pos, Renderer::CursorType cursor) {
    this->batch.draw(this->cursorRats[static_cast<int>(cursor)], pos);
}

void Renderer::box(sf::FloatRect pos, int highlight) {
    this->batch.draw(
        highlight ? this->boxHighlightPatch : this->boxPatch,
        pos
    );
}

void Renderer::panel(sf::FloatRect pos) {
    this->batch.draw(this->panelPatch, pos);
}

void Renderer::button(sf::FloatRect pos, int depressed) {
    this->batch.draw(
        depressed ? this->buttonDepressedPatch : this->buttonPatch,
        pos
    );
}

void Renderer::line(sf::Vector2f start, sf::Vector2f end, int highlight) {
    this->batch.draw(
        highlight ? this->lineHighlightRat : this->lineRat,
        start,
        end
    );
}

void Renderer::club(sf::Vector2f start, sf::Vector2f end, int highlight) {
    this->line(start, end, highlight);
    this->node(start, highlight);
}

void Renderer::sphereMesh(Mesh const &mesh, sf::Vector2f camera, int highlight) {
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

void Renderer::text(std::string const &content, sf::Vector2f pos) {
    float originX = pos.x;
    for (int i = 0; i < content.size(); i++) {
        if (content[i] == '\n') {
            pos.x = originX;
            pos.y += this->font.character.y;
            continue;
        }
        this->batch.draw(
            this->font.get(content[i]),
            sf::FloatRect(
                pos.x,
                pos.y,
                this->font.character.x,
                this->font.character.y
            )
        );
        pos.x += this->font.character.x - 1;
    }
}

void Renderer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(this->batch);
}
