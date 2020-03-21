#include "Memory.hh"

Memory::Memory(int id) {
    this->id = id;
}

int Memory::getId() const {
    return this->id;
}

int Memory::getSwitch(char const *name) const {
    if (this->switches.count(name) == 1) return this->switches.at(name);
    return false;
}

void Memory::setSwitch(char const *name, int value) {
    this->switches[name] = value;
}

int Memory::getLocalSwitch(Level const &level, char const *name) const {
    char const *levelName = level.file.c_str();
    if (this->localSwitches.count(levelName) == 1 &&
        this->localSwitches[levelName].count(name) == 1
    ) {
        return this->localSwitches[levelName][name];
    }
    return false;
}

void Memory::setLocalSwitch(Level const &level, char const *name, int value) {
    this->localSwitches[level.file.c_str()][name] = value;
}

int Memory::getItemCount(char const *item) const {
    if (this->items.count(item) == 1) {
        return this->items.at(item);
    }
    return 0;
}

void Memory::setItemCount(char const *item, int count) {
    this->items[item] = count;
}

std::unordered_map<std::string, int> const &Memory::getItems() const {
    return this->items;
}

std::unordered_map<std::string, int> const &Memory::getSwitches() const {
    return this->switches;
}
