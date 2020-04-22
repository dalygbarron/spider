#include "Camera.hh"
#include "Util.hh"
#include "spdlog/spdlog.h"

Camera::Camera() {
    this->set(0, 0);
}

void Camera::set(float longitude, float latitude) {
    this->angle = sf::Vector2f(longitude, latitude);
    this->to = Util::inverseRotationMatrix(
        sf::Vector3f(latitude, longitude, 0)
    );
    this->from = Util::rotationMatrix(
        sf::Vector3f(latitude, longitude, 0)
    );
}

void Camera::set(sf::Vector2f coordinate) {
    this->set(coordinate.x, coordinate.y);
}

void Camera::add(float longitude, float latitude) {
    this->set(this->angle.x + longitude, this->angle.y + latitude);
}

sf::Vector2f Camera::getAngle() const {
    return this->angle;
}

Matrix const &Camera::toMatrix() const {
    return this->to;
}

Matrix const &Camera::fromMatrix() const {
    return this->from;
}
