#include "Level.hh"

LevelInstance::LevelInstance(Level const &level) {
    this->script = level.script;
    this->picFile = level.picFile;
    for (Instance &instance: level.instance) {
        this->instances.push_back(std::copy(instance));
    }
    this->pic.loadFromFile(level.picFile.c_str());
    this->pic.setRepeated(true);
    this->pic.setSmooth(true);
}
