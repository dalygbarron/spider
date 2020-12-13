#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "RatPack.hh"
#include "Text.hh"
#include "spdlog/spdlog.h"
#include "pugixml.hpp"
#include <SFML/Audio.hpp>
#include <unordered_map>

/**
 * Loads and then stores assets.
 */
template <class T> class Repository {
    public:
        /**
         * Frees all the assets.
         */
        virtual ~Repository() {
            this->items.clear();
        }

        /**
         * Gets you an asset of the T type with the given name, and if it's not
         * already stored it is loaded in and then stored.
         * @param key is the name under which to look for the item.
         * @return the thing or null if it could not be found.
         */
        T *get(char const *key) {
            if (this->items.count(key) == 0) {
                T *item = this->load(key);
                if (item) {
                    this->items[key] = item;
                    return item;
                }
                spdlog::error("Could not load item with key '{}'", key);
                return NULL;
            }
            return this->items[key];
        }

        /**
         * Deletes something out of the cache so that the next time it is
         * requested it will be loaded from scratch.
         * @param key is the key to the thing to be removed.
         */
        void clear(char const *key) {
            this->items.erase(key);
        }

        /**
         * Load an asset with the given key.
         * @param key is the key of what to load.
         * @return the loaded item or null if it could not be loaded.
         */
        virtual T *load(char const *key) const = 0;

    private:
        std::unordered_map<std::string, T *> items;
};

/**
 * Repository for sounds.
 */
class SoundBufferRepository: public Repository<sf::SoundBuffer> {
    public:
        virtual sf::SoundBuffer *load(char const *key) const override;
};

/**
 * This repository takes care of strings. The idea with this is mostly so that
 * the editor can edit a string in memory and then other thingos will load it.
 */
class TextRepository: public Repository<Text> {
    private:
        virtual Text *load(char const *key) const override;
};

#endif
