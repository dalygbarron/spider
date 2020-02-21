#include "Level.hh"
#include "Util.hh"

Level::~Level() {
    for (Instance *instance: this->instances) delete instance;
}

void Level::setPic(ghc::filesystem::path const &path) {
    this->pic.loadFromFile(path.c_str());
    this->picFile = path;
}

sf::Texture const &Level::getPic() const {
    return this->pic;
}

ghc::filesystem::path const &Level::getPicFile() const {
    return this->picFile;
}

void Level::addInstance(Instance *instance) {
    this->instances.push_back(instance);
}
