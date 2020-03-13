#ifndef ITEM_H
#define ITEM_H

/**
 * Prototypal item.
 */
class Item {
    public:
        std::string const name;
        std::string const description;
        sf::IntRect const rat;

        /**
         * Creates the item and sets the stuff.
         * @param name        is the name of the item.
         * @param description is a textual description of the item.
         * @param rat         is the icon for the item.
         */
        Item(char const *name, char const *description, sf::IntRect rat);
};

#endif
