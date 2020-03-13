#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include <string>

/**
 * Prototypal item.
 */
class Item {
    public:
        std::string name;
        std::string displayName;
        std::string description;
        sf::IntRect rat;
};

#endif
