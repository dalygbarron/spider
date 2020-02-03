#include "Mesh.hh"

void Mesh::addVertex(sf::Vector2f vertex) {
    this->vertices.push_back(vertex);
}

std::vector<sf::Vector2f> const &Mesh::getVertices() const {
    return this->vertices;
}

int Mesh::in(sf::Vector2f pos) const {
    int inside = false;
    // TODO: might not be counting the final line that connects it up.
    for (int i = 0; i < this->vertices.size() - 1; i++) {
        int intersect = ((this->vertices[i].y > pos.y) !=
            (this->vertices[i + 1].y > pos.y)) &&
            (pos.x < (this->vertices[i + 1].x - this->vertices[i].x) *
            (pos.y - this->vertices[i].y) /
            (this->vertices[i + 1].y - this->vertices[i].y) +
            this->vertices[i].x);
        if (intersect) inside = !inside;
    }
    return inside;
}
