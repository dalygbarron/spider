#include "Level.hh"
#include "Util.hh"
#include "spdlog/spdlog.h"

Level::Level() {
    this->pic.setSmooth(true);
}

void Level::setPic(ghc::filesystem::path const &path) {
    this->pic.loadFromFile(path.c_str());
    this->pic.setRepeated(true);
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
