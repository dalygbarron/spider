#include "Memory.hh"

Memory::Memory(int id): id(id) {
    // nothing else.
}

int Memory::getSwitch(char const *name) const {
    if (this->switches.count(name) == 1) return this->switches[name];
    return false;
}

void Memory::setSwitch(char const *name, int value) {
    this->switches[name] = value;
}

int Memory::getItemCount(Item const &item) const {
    if (this->items.count(item.name) == 1) {
        return this->items[item.name];
    }
    return 0;
}

void Memory::setItemCount(Item const &item, int count) {
    this->items[item.name] = count;
}

unordered_map<std::string, int> const &Memory::getSwitches() const {
    return this->switches;
}

unordered_map<std::string, int> const &Memory::getItems() const {
    return this->items;
}
