#include "Mesh.hh"
#include "Const.hh"
#include "Util.hh"
#include "spdlog/spdlog.h"
#include <cmath>

void Mesh::addVertex(glm::vec2 vertex) {
    this->vertices.push_back(vertex);
}

void Mesh::split(int index) {
    if (index < 0 || index >= this->vertices.size()) return;
    glm::vec2 vertex = this->vertices[index];
    int next = index + 1;
    if (next >= this->vertices.size()) next = 0;
    vertex.x += this->vertices[next].x;
    vertex.y += this->vertices[next].y;
    vertex.x /= 2;
    vertex.y /= 2; this->vertices.insert(this->vertices.begin() + index + 1, vertex);
}

void Mesh::remove(int index) {
    this->vertices.erase(this->vertices.begin() + index);
}

glm::vec2 Mesh::getCentre() {
    glm::vec2 total;
    for (glm::vec2 vertex: this->vertices) total += vertex;
    int n = this->vertices.size();
    return glm::vec2(total.x / n, total.y / n);
}

glm::vec2 *Mesh::getVertex(int index) {
    if (index < 0 || index >= this->vertices.size()) return NULL;
    return this->vertices.data() + index;
}

std::vector<glm::vec2> const &Mesh::getVertices() const {
    return this->vertices;
}

int Mesh::in(glm::vec2 pos) const {
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

int Mesh::inSphere(glm::vec2 coordinate) const {
    coordinate.x = fmod(coordinate.x, Const::DOUBLE_PI);
    coordinate.y = fmod(coordinate.y, Const::PI);
    int size = this->vertices.size();
    if (size < 3) return false;
    int inside = false;
    for (int i = 0; i < size; i++) {
        int next = i + 1;
        if (next == size) next = 0;
        float height = (this->vertices[next].y - this->vertices[i].y) *
            fabs(coordinate.x - this->vertices[i].x) / 
            fabs(this->vertices[next].x - this->vertices[i].x) +
            this->vertices[i].y;
        int intersect = false;
        int aligned = Util::inSlice(
            this->vertices[i].x,
            this->vertices[next].x,
            coordinate.x
        );
        if (!aligned) continue;
        if (coordinate.y < 0) intersect = height >= coordinate.y;
        else intersect = height <= coordinate.y;
        if (intersect) {
            inside = !inside;
        }
    }
    return inside;
}

int Mesh::getClosestVertex(glm::vec2 pos, float threshold) const {
    int index = -1;
    float distance = threshold;
    for (int i = 0; i < this->vertices.size(); i++) {
        // Manhattan distance will do.
        float vertexDistance = fabs(this->vertices[i].x - pos.x) +
            fabs(this->vertices[i].y - pos.y);
        if (vertexDistance <= distance) {
            index = i;
            distance = vertexDistance;
        }
    }
    return index;
}
