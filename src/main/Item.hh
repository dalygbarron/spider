#ifndef ITEM_H
#define ITEM_H

#include "Rectangle.hh"
#include <string>

/**
 * Prototypal item.
 */
class Item {
    public:
        std::string name;
        std::string displayName;
        std::string description;
        std::string rat;
        Rectangle sprite;
};

#endif
