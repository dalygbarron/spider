#include "Memory.hh"

Memory::Memory(int nItems) {
    this->inventory = new int[nItems];
}

Memory::~Memory() {
    delete this->inventory;
}

int Memory::getSwitch(char const *name) const {
    // TODO: check whether switch exists and if not return false.
    return true;
}

void Memory::setSwitch(char const *name, int value) {
    this->switches[name] = value;
}
