#include "Util.hh"
#include "Const.hh"
#include "spdlog/spdlog.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <cmath>

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

float Util::degrees(float radians) {
    return fmod(radians, Const::DOUBLE_PI) * Const::RADIAN_CONVERT;
}

float Util::distance(sf::Vector2f a, sf::Vector2f b) {
    float dX = abs(a.x - b.x);
    float dY = abs(a.y - b.y);
    return sqrt(dX * dX + dY * dY);
}

float Util::length(sf::Vector3f vector) {
    return sqrt(
        vector.x * vector.x + vector.y * vector.y + vector.z * vector.z
    );
}

sf::Vector3f Util::rotate(sf::Vector3f vector, sf::Vector2f angle) {
    sf::Vector3f rotated;
    float sideLength = sqrt(vector.z * vector.z + vector.y * vector.y);
    float sideAngle = atan2(vector.y, vector.z);
    rotated.z = cos(sideAngle - angle.y) * sideLength;
    rotated.y = sin(sideAngle - angle.y) * sideLength;
    rotated.x = vector.x;
    float topLength = sqrt(rotated.x * rotated.x + rotated.z * rotated.z);
    float topAngle = atan2(rotated.z, rotated.x);
    rotated.x = cos(topAngle - angle.x) * topLength;
    rotated.z = sin(topAngle - angle.x) * topLength;
    return rotated;
}

float Util::manhattan(sf::Vector2f a, sf::Vector2f b) {
    return abs(a.x - b.x) + abs(a.y + b.y);
}

sf::Vector3f sphereToPoint(sf::Vector2f coordinate) {
    return sf::Vector3f(
        cos(coordinate.y) * cos(coordinate.x),
        sin(coordinate.y),
        cos(coordinate.y) * sin(coordinate.x)
    );
}

sf::Vector2f pointToSphere(sf::Vector3f point) {
    return sf::Vector2f(
        atan(pos.z / pos.x),
        atan(pos.y / sqrt(pos.x * pos.x + pos.z * pos.z))
    );
}

sf::Vector2f Util::sphereToScreen(
    sf::Vector2f coordinate,
    sf::Vector2f camera
) {
    sf::Vector2f fov(2.094395, 1.570796);
    sf::Vector2f length(sin(fov.x / 2), sin(fov.y / 2));
    sf::Vector2f distance(cos(fov.x / 2), cos(fov.y / 2));
    coordinate = sphereToPoint(rotate(sphereToPoint(coordinate), camera));
    return sf::Vector2f(
        (tan(coordinate.x) * distance.x) / length.x * Const::WIDTH / 2 + Const::WIDTH / 2,
        (tan(coordinate.y) * distance.y) / length.y * Const::HEIGHT / 2 + Const::HEIGHT / 2
    );
}
