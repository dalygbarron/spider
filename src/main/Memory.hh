#ifndef MEMORY_H
#define MEMORY_H

/**
 * Stores the state of stuff that is remembered long term as in stuff the
 * player has done etc.
 */
class Memory {
    public:
        int getSwitch(char const *name);

    private:
        int id;
        unordered_map<std::string, int> switches;

};

#endif
