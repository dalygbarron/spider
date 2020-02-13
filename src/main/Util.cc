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
    return abs(a.x - b.x) + abs(a.y + b.y);
}

sf::Vector2f Util::sphereToScreen(
    sf::Vector2f coordinate,
    sf::Vector2f camera
) {
    coordinate = rotate(coordinate, sf::Vector2f(-camera.x, -camera.y));
    if (cos(coordinate.x) < 0 || cos(coordinate.y) < 0) return sf::Vector2f(-99999, -99999);
    return sf::Vector2f(
        tan(coordinate.x) * Const::INVERSE_RENDER_LENGTH_X * Const::WIDTH + Const::HALF_WIDTH,
        tan(coordinate.y) * Const::INVERSE_RENDER_LENGTH_Y * Const::HEIGHT + Const::HALF_HEIGHT
    );
}
