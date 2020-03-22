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

int Memory::getLocalSwitch(char const *space, char const *name) const {
    if (this->localSwitches.count(space) == 1 &&
        this->localSwitches.at(space).count(name) == 1
    ) {
        return this->localSwitches.at(space).at(name);
    }
    return false;
}

void Memory::setLocalSwitch(char const *space, char const *name, int value) {
    this->localSwitches[space][name] = value;
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

std::unordered_map<std::string, std::unordered_map<std::string, int>>
    const &Memory::getLocalSwitches() const
{
    return this->localSwitches;
}
