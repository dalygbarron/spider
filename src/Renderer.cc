#include "Renderer.hh"

Renderer::Renderer(sf::Texture const &sprites): batch(sprites) {
    // does nothing else.
}

void Renderer::setPointRat(sf::IntRect rat) {
    this->pointRat = rat;
}

void Renderer::setNodeRat(sf::IntRect rat) {
    this->nodeRat = rat;
}

void Renderer::setNodeHighlightRat(sf::IntRect rat) {
    this->nodeHighlightRat = rat;
}

void Renderer::setBoxRat(sf::IntRect rat) {
    this->boxRat = rat;
}

void Renderer::setBoxHighlightRat(sf::IntRect rat) {
    this->boxHighlightRat = rat;
}

void Renderer::point(sf::Vector2f pos) {
    this->batch.draw(this->pointRat, pos);
}

void Renderer::node(sf::Vector2f pos, int highlight) {
    this->batch.draw(
        highlight ? this->nodeHighlightRat : this->nodeRat,
        pos
    );
}

void Renderer::box(sf::FloatRect pos, int highlight) {
    this->batch.draw(
        highlight ? this->boxHighlightRat : this->boxRat,
        pos
    );
}

void Renderer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(this->batch);
}
