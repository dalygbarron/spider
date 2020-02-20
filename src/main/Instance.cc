#include "Instance.hh"
#include "Util.hh"
#include <float.h>

float Instance::distance(sf::Vector2f coordinate) const {
    if (this->entity) return Util::distance(this->pos, coordinate);
    int index = this->mesh.getClosestVertex(coordinate, 1);
    if (index == -1) return FLT_MAX;
    sf::Vector2f vertex = this->mesh.getVertices()[index];
    return Util::distance(vertex, coordinate);
}
