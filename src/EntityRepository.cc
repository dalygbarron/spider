#include "EntityRepository.hh"

Entity *EntityRepository::load(char const *key) const {
    ghc::filesystem::path file = key;
    if (ghc::filesystem::exists(file)) {
        // TODO: loading in the xml stuff.
        return NULL;
    }
    Entity *entity = new Entity();
    entity->file = file;
    return entity;
}
