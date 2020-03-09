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
    float dX = fabs(a.x - b.x);
    float dY = fabs(a.y - b.y);
    return sqrt(dX * dX + dY * dY);
}

float Util::length(sf::Vector3f vector) {
    return sqrt(
        vector.x * vector.x + vector.y * vector.y + vector.z * vector.z
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
    return fabs(a.x - b.x) + fabs(a.y + b.y);
}

sf::Vector2f Util::screenToSphere(
    sf::Vector2f pos,
    sf::Vector2f camera
) {
    // yeah, dunno why but for this you have to rotate around the y axis first
    // and then the x axis, but for the rotate function it was the other way
    // round in order to work so who knows lol.
    sf::Vector2f coordinate(
        atan((pos.x - Const::HALF_WIDTH) /
            Const::WIDTH * Const::RENDER_LENGTH_X),
        atan((pos.y - Const::HALF_HEIGHT) /
            Const::HEIGHT * Const::RENDER_LENGTH_Y)
    );
    float cosY = cos(coordinate.y);
    sf::Vector3f vector(
        cosY * sin(coordinate.x),
        sin(coordinate.y),
        cos(coordinate.x) * cosY
    );
    float sideLength = sqrt(vector.z * vector.z + vector.y * vector.y);
    float sideAngle = atan2(vector.y, vector.z);
    vector.z = cos(sideAngle + camera.y) * sideLength;
    vector.y = sin(sideAngle + camera.y) * sideLength;
    return sf::Vector2f(
        atan2(vector.x, vector.z) + camera.x,
        atan2(vector.y, sqrt(vector.x * vector.x + vector.z * vector.z))
    );
}

sf::Vector3f Util::sphereToScreen(
    sf::Vector2f coordinate,
    sf::Vector2f camera
) {
    coordinate = rotate(coordinate, sf::Vector2f(-camera.x, -camera.y));
    return sf::Vector3f(
        tan(coordinate.x) * Const::INVERSE_RENDER_LENGTH_X * Const::WIDTH +
            Const::HALF_WIDTH,
        tan(coordinate.y) * Const::INVERSE_RENDER_LENGTH_Y * Const::HEIGHT +
            Const::HALF_HEIGHT,
        cos(coordinate.x)
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

sf::Vector2f Util::rotateAround(
    sf::Vector2f pos,
    sf::Vector2f origin,
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
