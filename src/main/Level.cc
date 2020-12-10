#include "Level.hh"
#include "Util.hh"
#include "spdlog/spdlog.h"

LevelInstance::LevelInstance(Level const &level): level(level) {
    this->pic.loadFromFile(level.getPicFile().c_str());
    this->pic.setRepeated(true);
    this->pic.setSmooth(true);
}

void Level::setPic(ghc::filesystem::path const &path) {
    this->picFile = path;
}

sf::Texture const &Level::getPic() const {
    return this->pic;
}

ghc::filesystem::path const &Level::getPicFile() const {
    return this->picFile;
}

Instance &Level::addInstance() {
    int n = this->instances.size();
    this->instances.resize(n + 1);
    return this->instances[n];
}
