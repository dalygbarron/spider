#include "Util.hh"
#include "Const.hh"
#include "spdlog/spdlog.h"
#include <SFML/Graphics.hpp>
#include <glm/ext.hpp>
#include <stdio.h>
#include <cmath>

void Util::sleep(double delta) {
    static constexpr std::chrono::duration<double> minSleepDuration(0);
    std::chrono::high_resolution_clock::time_point start =
        std::chrono::high_resolution_clock::now();
    while (std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - start
        ).count() < delta
    ) {
        std::this_thread::sleep_for(minSleepDuration);
    }
}

sf::View Util::getLetterboxView(sf::View view, sf::Vector2i dimensions) {
    float windowRatio = dimensions.x / (float)dimensions.y;
    float viewRatio = view.getSize().x / (float)view.getSize().y;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;
    int horizontalSpace = true;
    if (windowRatio < viewRatio) horizontalSpace = false;
    if (horizontalSpace) {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) / 2.f;
    } else {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.f;
    }
    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    return view;
}

void Util::centreMouse(sf::Window &window) {
    sf::Mouse::setPosition(Const::MOUSE_ORIGIN, window);
}

float Util::degrees(float radians) {
    return fmod(radians, Const::DOUBLE_PI) * Const::RADIAN_CONVERT;
}

float Util::distance(sf::Vector2f a, sf::Vector2f b) {
    float dX = fabs(a.x - b.x);
    float dY = fabs(a.y - b.y);
    return sqrt(dX * dX + dY * dY);
}

float Util::distance3(sf::Vector3f a, sf::Vector3f b) {
    float dX = fabs(a.x - b.x);
    float dY = fabs(a.y - b.y);
    float dZ = fabs(a.z - b.z);
    return sqrt(dX * dX + dY * dY + dZ * dZ);
}

float Util::length(sf::Vector3f vector) {
    return sqrt(
        vector.x * vector.x + vector.y * vector.y + vector.z * vector.z
    );
}

float Util::dotProduct(sf::Vector3f a, sf::Vector3f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

sf::Vector2f Util::normalise(sf::Vector2f in) {
    float length = sqrt(in.x * in.x + in.y * in.y);
    if (length > 0) {
        float inverseLength = 1 / length;
        in.x *= inverseLength;
        in.y *= inverseLength;
    }
    return in;
}

sf::Vector3f Util::normalise(sf::Vector3f in) {
    float squareLength = Util::dotProduct(in, in);
    if (squareLength > 0) {
        float inverseLength(1 / sqrt(squareLength));
        in.x *= inverseLength;
        in.y *= inverseLength;
        in.z *= inverseLength;
    }
    return in;
}

sf::Vector3f Util::crossProduct(sf::Vector3f a, sf::Vector3f b) {
    return sf::Vector3f(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

int Util::inSlice(float a, float b, float point) {
    a = fmod(a, Const::DOUBLE_PI);
    b = fmod(b, Const::DOUBLE_PI);
    point = fmod(point, Const::DOUBLE_PI);
    float delta = fabs(a - b);
    if (delta <= Const::PI) {
        return (point >= a && point <= b) || (point >= b && point <= a);
    }
    return (point >= a && point >= b) || (point <= a && point <= b);
}

sf::Vector2f Util::clampInRect(sf::Vector2f point, sf::FloatRect rect) {
    if (point.x < rect.left) {
        point.x = rect.left;
    } else if (point.x >= rect.left + rect.width) {
        point.x = rect.left + rect.width;
    }
    if (point.y < rect.top) {
        point.y = rect.top;
    } else if (point.y >= rect.top + rect.height) {
        point.y = rect.top + rect.height;
    }
    return point;
}

sf::Vector2f Util::rotate(sf::Vector2f coordinate, sf::Vector2f angle) {
    float cosY = cos(coordinate.y);
    sf::Vector3f vector(
        cosY * sin(coordinate.x + angle.x),
        sin(coordinate.y),
        cos(coordinate.x + angle.x) * cosY
    );
    float sideLength = sqrt(vector.z * vector.z + vector.y * vector.y);
    float sideAngle = atan2(vector.y, vector.z);
    vector.z = cos(sideAngle + angle.y) * sideLength;
    vector.y = sin(sideAngle + angle.y) * sideLength;
    return sf::Vector2f(
        atan2(vector.x, vector.z),
        atan2(vector.y, sqrt(vector.x * vector.x + vector.z * vector.z))
    );
}

float Util::manhattan(sf::Vector2f a, sf::Vector2f b) {
    return fabs(a.x - b.x) + fabs(a.y - b.y);
}

float Util::manhattan3(sf::Vector3f a, sf::Vector3f b) {
    return fabs(a.x - b.x) + fabs(a.y - b.y) + fabs(a.z - b.z);
}

sf::Vector3f Util::toSphere(sf::Vector3f pos, sf::Vector3f camera) {
    sf::Vector3f delta = pos - camera;
    float distance = Util::distance3(pos, camera);
    if (distance == 0) return sf::Vector3f(0, 0, 0);
    return sf::Vector3f(
        atan2(delta.y, delta.x),
        asin(delta.z / distance),
        distance
    );
}

float Util::upAngle(
    sf::Vector2f camera,
    sf::Vector2f floor,
    sf::Vector2f pos
) {
    if (camera.y > 0) {
        return atan2(floor.x - pos.x, floor.y - pos.y) * sin(camera.y) * -1;
    } else {
        return atan2(pos.x - floor.x, pos.y - floor.y) * sin(camera.y);
    }
}

glm::vec2 Util::rotateAround(
    glm::vec2 pos,
    glm::vec2 origin,
    float angle,
    float scale
) {
    pos.x -= origin.x;
    pos.y -= origin.y;
    float length = sqrt(pos.x * pos.x + pos.y * pos.y) * scale;
    float currentAngle = atan2(pos.y, pos.x);
    pos.x = cos(currentAngle + angle) * length;
    pos.y = sin(currentAngle + angle) * length;
    pos.x += origin.x;
    pos.y += origin.y;
    return pos;
}

glm::mat4 Util::camera(glm::vec2 angle) {
    glm::mat4 projection = glm::perspective(Const::FOV, 1.5, 0.1, 100);
    glm::mat4 view = glm::rotate(glm::mat4(1), angle.y, glm::vec3(-1, 0, 0));
    view = glm::rotate(view, angle.x, glm::vec3(0, 1, 0));
    return projection * view;
}

glm::vec3 Util::sphericalToCartesian(glm::vec2 spherical) {
    return glm::vec3(
        cos(spherical.y) * sin(spherical.x),
        sin(spherical.y),
        cos(spherical.y) * -cos(spherical.x)
    );
}

glm::vec2 Util::cartesianToSpherical(glm::vec3 cartesian) {
    return glm::vec2(
        atan2(cartesian.z, cartesian.x) + Const::HALF_PI,
        asin(cartesian.y)
    );
}
