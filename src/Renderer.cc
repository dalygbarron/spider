#include "Renderer.hh"

Renderer::Renderer(sf::Texture const &sprites): batch(sprites) {
    // does nothing else.
}

void Renderer::setPointRat(sf::IntRect rat) {
    this->pointRat = rat;
}

void Renderer::setLineRat(sf::IntRect rat) {
    this->lineRat = rat;
}

void Renderer::setLineHighlightRat(sf::IntRect rat) {
    this->lineHighlightRat = rat;
}

void Renderer::setNodeRat(sf::IntRect rat) {
    this->nodeRat = rat;
}

void Renderer::setNodeHighlightRat(sf::IntRect rat) {
    this->nodeHighlightRat = rat;
}

void Renderer::setBoxPatch(Patch patch) {
    this->boxPatch = patch;
}

void Renderer::setBoxHighlightPatch(Patch patch) {
    this->boxHighlightPatch = patch;
}

void Renderer::point(sf::Vector2f pos) {
    this->batch.draw(this->pointRat, pos);
}

void Renderer::node(sf::Vector2f pos, int highlight) {
    this->batch.draw(highlight ? this->nodeHighlightRat : this->nodeRat, pos);
}

void Renderer::box(sf::FloatRect pos, int highlight) {
    this->batch.draw(
        highlight ? this->boxHighlightPatch : this->boxPatch,
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

void Renderer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(this->batch);
}
