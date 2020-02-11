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

sf::Vector3f Util::rotate(sf::Vector3f vector, sf::Vector2f angle) {
    sf::Vector2f cosines(cos(angle.x), cos(angle.y));
    sf::Vector2f sines(sin(angle.x), sin(angle.y));
    sf::Vector3f p(
        vector.x,
        cosines.x * vector.y + sines.x * vector.z,
        -sines.x * vector.y + cosines.x * vector.z
    );
    return sf::Vector3f(
        cosines.y * p.x + sines.y * p.z,
        p.y,
        -sines.y * p.x + cosines.y * p.z
    );
}

float Util::manhattan(sf::Vector2f a, sf::Vector2f b) {
    return abs(a.x - b.x) + abs(a.y + b.y);
}

sf::Vector2f Util::sphereToScreen(
    sf::Vector2f coordinate,
    sf::Vector2f camera
) {
    sf::Vector2f fov(2.094395, 1.570796);
    sf::Vector2f length(sin(fov.x / 2), sin(fov.y / 2));
    sf::Vector2f distance(cos(fov.x / 2), cos(fov.y / 2));
    sf::Vector3f pos(
        cos(coordinate.y) * cos(coordinate.x),
        cos(coordinate.y) * sin(coordinate.x),
        sin(coordinate.y)
    );
    pos = Util::rotate(pos, camera);
    coordinate.x = atan2(pos.y, pos.x);
    coordinate.y = atan2(pos.z, sqrt(pos.x * pos.x + pos.y * pos.y));
    spdlog::info(
        "lat ({} {}) cam ({} {}) pos ({} {} {})",
        coordinate.x,
        coordinate.y,
        camera.x,
        camera.y,
        pos.x,
        pos.y,
        pos.z
    );
    return sf::Vector2f(
        (tan(coordinate.x) * distance.x) / length.x * Const::WIDTH / 2 + Const::WIDTH / 2,
        (tan(coordinate.y) * distance.y) / length.y * Const::HEIGHT / 2 + Const::HEIGHT / 2
    );
}
