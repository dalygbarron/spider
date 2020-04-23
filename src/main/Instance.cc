#include "Instance.hh"
#include "Util.hh"
#include <float.h>

float Instance::distance(glm::vec2 coordinate) const {
    if (this->entity) return glm::distance(this->pos, coordinate);
    int index = this->mesh.getClosestVertex(coordinate, 1);
    if (index == -1) return FLT_MAX;
    glm::vec2 vertex = this->mesh.getVertices()[index];
    return glm::distance(vertex, coordinate);
}
