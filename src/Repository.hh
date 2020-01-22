#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <unordered_map>

/**
 * Loads and then stores assets.
 */
class Repository<T> {
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
         * Load an asset with the given key.
         * @param key is the key of what to load.
         * @return the loaded item or null if it could not be loaded.
         */
        virtual T *load(char const *key) const = 0;

    private:
        std::unordered_map<std::string, T *> items;
};

#endif
