#include "Mesh.hh"
#include <cmath>

void Mesh::addVertex(sf::Vector2f vertex) {
    this->vertices.push_back(vertex);
}

void Mesh::split(int index) {
    if (index < 0 || index >= this->vertices.size()) return;
    sf::Vector2f vertex = this->vertices[index];
    int next = index + 1;
    if (next >= this->vertices.size()) next = 0;
    vertex.x += this->vertices[next].x;
    vertex.y += this->vertices[next].y;
    vertex.x /= 2;
    vertex.y /= 2;
    this->vertices.insert(this->vertices.begin() + index + 1, vertex);
}

std::vector<sf::Vector2f> const &Mesh::getVertices() const {
    return this->vertices;
}

int Mesh::in(sf::Vector2f pos) const {
    int size = this->vertices.size();
    if (size < 3) return false;
    int inside = false;
    for (int i = 0; i < size; i++) {
        int next = i + 1;
        if (next == size) next = 0;
        int intersect = ((this->vertices[i].y > pos.y) !=
            (this->vertices[next].y > pos.y)) &&
            (pos.x < (this->vertices[next].x - this->vertices[i].x) *
            (pos.y - this->vertices[i].y) /
            (this->vertices[next].y - this->vertices[i].y) +
            this->vertices[i].x);
        if (intersect) inside = !inside;
    }
    return inside;
}

int Mesh::getClosestEdge(sf::Vector2f pos) const {
    int index = -1;
    float distance = 999999999;
    int n = this->vertices.size();
    for (int i = 0; i < n; i++) {
        // Manhattan distance will do.
        float vertexDistance = abs(this->vertices[i].x - pos.x) +
            abs(this->vertices[i].y - pos.y);
        if (vertexDistance < distance && vertexDistance < Mesh::CLOSE) {
            index = i;
            distance = vertexDistance;
        }
    }
    if (index == -1) return index;
    int left = index - 1;
    int right = index + 1;
    if (left < 0) left = n - 1;
    if (right >= n) right = 0;
    sf::Vector2f leftPos = this->vertices[left];
    sf::Vector2f rightPos = this->vertices[right];
    float leftNodeDistance = Util::manhattan(leftPos, this->vertices[index]);
    float rightNodeDistance = Util::manhattan(leftPos, this->vertices[index]);
    return index;
}
