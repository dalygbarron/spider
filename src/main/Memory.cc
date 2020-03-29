#include "Memory.hh"

Memory::SwitchExpression::SwitchExpression(
    SwitchExpression::Type type,
    char const *name,
    Memory::SwitchExpression *a,
    Memory::SwitchExpression *b
) {
    this->type = type;
    if (name) this->name = name;
    this->a = a;
    this->b = b;
}

Memory::SwitchExpression::~SwitchExpression() {
    if (this->a) delete this->a;
    if (this->b) delete this->b;
}

int Memory::SwitchExpression::evaluate(Memory const &memory) const {
    switch (this->type) {
        case Memory::SwitchExpression::Type::Switch:
            return memory.getSwitch(this->name.c_str());
        case Memory::SwitchExpression::Type::LocalSwitch:
            return memory.getLocalSwitch(this->name.c_str());
        case Memory::SwitchExpression::Type::And:
            return this->a->evaluate(memory) && this->b->evaluate(memory);
        case Memory::SwitchExpression::Type::Or:
            return this->a->evaluate(memory) || this->b->evaluate(memory);
        case Memory::SwitchExpression::Type::Not:
            return !this->a->evaluate(memory);
        default:
            return false;
    }
}

Memory::Memory(unsigned int id) {
    this->id = id;
}

unsigned int Memory::getId() const {
    return this->id;
}

int Memory::getSwitch(char const *name) const {
    if (this->switches.count(name) == 1) return this->switches.at(name);
    return false;
}

void Memory::setSwitch(char const *name, int value) {
    this->switches[name] = value;
}

int Memory::getLocalSwitch(char const *name) const {
    char const *space = this->level.c_str();
    if (this->localSwitches.count(space) == 1 &&
        this->localSwitches.at(space).count(name) == 1
    ) {
        return this->localSwitches.at(space).at(name);
    }
    return false;
}

void Memory::setLocalSwitch(char const *name, int value) {
    this->localSwitches[this->level.c_str()][name] = value;
}

void Memory::setLocalSwitchStatic(
    char const *locale,
    char const *name,
    int value
) {
    this->localSwitches[locale][name] = value;
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

char const *Memory::getString(char const *name) const {
    if (this->strings.count(name) == 1) {
        return this->strings.at(name).c_str();
    }
    return "";
}

void Memory::setString(char const *name, char const *value) {
    this->strings[name] = value;
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
