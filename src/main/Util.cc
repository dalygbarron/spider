#include "Util.hh"
#include "Const.hh"
#include "spdlog/spdlog.h"
#include "glm/gtc/matrix_transform.hpp"
#include <SFML/Graphics.hpp>
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

int Util::findString(char const *string, char const * const *in, int n) {
    for (int i = 0; i < n; i++) {
        if (strcmp(string, in[i]) == 0) return i;
    }
    return -1;
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

float Util::manhattan3(glm::vec3 a, glm::vec3 b) {
    return fabs(a.x - b.x) + fabs(a.y - b.y) + fabs(a.z - b.z);
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
    glm::mat4 view(1);
    view = glm::rotate(view, angle.y, glm::vec3(1, 0, 0));
    view = glm::rotate(view, angle.x, glm::vec3(0, 1, 0));
    return view;
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

glm::vec2 Util::screenToSpherical(
    glm::vec2 screen,
    glm::mat4 camera,
    glm::mat4 projection
) {
    glm::vec3 cartesian = glm::unProject(
        glm::vec3(screen, 1.0f),
        camera,
        projection,
        glm::vec4(0, 0, 1, 1)
    );
    return Util::cartesianToSpherical(cartesian);
}
