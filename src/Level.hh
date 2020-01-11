#ifndef LEVEL_H
#define LEVEL_H

#include "Entity.hh"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

/**
 * Represents a level.
 */
class Level {
    public:
        sf::Texture texture;
        std::string name;
        std::string script;
        std::unordered_map<std::string, Entity *> defines;
        std::unordered_map<std::string, Instance *> entities;
        std::unordered_map<std::string, std::vector<sf::Vector2f>> shapes;
};

#endif
