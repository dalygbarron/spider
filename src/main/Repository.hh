#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "Entity.hh"
#include "Level.hh"
#include "RatPack.hh"
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
 * A repository that is designed to load stuff out of xml files and thus does
 * so before passing on the unique logic so I don't need that usual xml file
 * fucking around.
 * \    /\
 *  )  ( ')
 * (  /  )
 *  \(__)|
 */
template <class T> class XmlRepository: public Repository<T> {
    public:
        virtual T *load(char const *key) const override {
            ghc::filesystem::path file = key;
            if (!ghc::filesystem::exists(file)) {
                return this->create(file);
            }
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_file(key);
            if (!result) {
                spdlog::error(
                    "couldn't open {} xml file '{}'",
                    this->getNodeName(),
                    key
                );
                return NULL;
            }
            pugi::xml_node node = doc.child("entity");
            if (!node) {
                spdlog::error(
                    "File '{}' lacks top '{}' node",
                    key,
                    this->getNodeName()
                );
                return NULL;
            }
            return this->parse(node, file);
        }

    private:
        /**
         * Creates a new item when the given file does no exist. This is
         * implemented by default to return null which basically means that
         * creation is not possible so if that is what you want just don't
         * override this.
         * @param path is the name of the file for this thing, which is
         *             also the key given but as a path relative to the root.
         * @return a pointer to the thing or NULL for no thing.
         */
        virtual T *create(ghc::filesystem::path const &path) const {
            return NULL;
        }

        /**
         * Used to query the name of the top level node to be looking for.
         * @return the name as a string.
         */
        virtual char const *getNodeName() const = 0;

        /**
         * function that does the actual parsing of the xml data.
         * @param node is the xml node to parse from.
         * @param path is the path to the file so it can be remembered.
         * @return the created thing if it could be created.
         */
        virtual T *parse(
            pugi::xml_node const &node,
            ghc::filesystem::path const &path
        ) const = 0;
};

/**
 * Repository for sounds.
 */
class SoundBufferRepository: public Repository<sf::SoundBuffer> {
    public:
        virtual sf::SoundBuffer *load(char const *key) const override;
};

/**
 * Repository for entities.
 */
class EntityRepository: public XmlRepository<Entity> {
    public:
        /**
         * Puts in the ratpack.
         * @param spritesheet is the ratpack that is used to give entities
         *                    their sprites. This means the spritesheet must be
         *                    fully formed from the start basically. If this
         *                    was problematic I could put the sprite in the
         *                    instance rather than the entity but really it's
         *                    not.
         */
        EntityRepository(RatPack const &spritesheet);

    private:
        RatPack const &spritesheet;

        virtual Entity *create(
            ghc::filesystem::path const &path
        ) const override;

        virtual char const *getNodeName() const override;

        virtual Entity *parse(
            pugi::xml_node const &node,
            ghc::filesystem::path const &path
        ) const override;
};

#endif
