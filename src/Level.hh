#ifndef LEVEL_H
#define LEVEL_H

/**
 * Represents a level.
 */
class Level {
    public:
        sf::Texture texture;
        std::string name;
        std::string script;
        std::HashMap<std::string, Entity *> entities;
        std::HashMap<std::string, vector<sf::Vector2f>> shapes;
};

#endif
