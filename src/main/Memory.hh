#ifndef MEMORY_H
#define MEMORY_H

#include "Level.hh"
#include <string>
#include <unordered_map>

/**
 * Stores the state of stuff that is remembered long term as in stuff the
 * player has done etc.
 */
class Memory {
    public:
        std::string level;

        /**
         * Creates the memory and sets it's id.
         * @param id is the id to give it.
         */
        Memory(int id);

        /**
         * Gives you the memory's id.
         * @return the id.
         */
        int getId() const;

        /**
         * Gives you the value of a switch.
         * @param name is the name of the switch to check the value of.
         * @return the value of the switch. If the given switch has never been
         * specified then it's value is considered to be false.
         */
        int getSwitch(char const *name) const;

        /**
         * Sets the value of a switch.
         * @param name  is the name of the switch to set.
         * @param value is the value to set it to.
         */
        void setSwitch(char const *name, int value);

        /**
         * Gives you the value of the given switch as set specifically for the
         * given level.
         * @param space is the unique identifier that these switches are in.
         * @param name  is the name of the switch to get.
         * @return the value of the switch, which is false if it has not been
         *         set.
         */
        int getLocalSwitch(char const *space, char const *name) const;

        /**
         * Sets the value of a given switch specifically for the given level.
         * @param space is unique identifier that these switches are in.
         * @param name  is the name of the switch to set.
         * @param value is the value to set the switch to.
         */
        void setLocalSwitch(char const *space, char const *name, int value);

        /**
         * Gives you the number that the player has of a given item.
         * @param item is the item to get the number of.
         * @return the number.
         */
        int getItemCount(char const *item) const;

        /**
         * Sets the number that the player has of a given item.
         * @param item  is to set the count of.
         * @param count is the number to set it to.
         */
        void setItemCount(char const *item, int count);

        /**
         * Gives you read only access to the full item list.
         * @return the item list.
         */
        std::unordered_map<std::string, int> const &getItems() const;

        /**
         * Gives you read only access to the full switch list.
         * @return the switch list.
         */
        std::unordered_map<std::string, int> const &getSwitches() const;

        /**
         * Gives you the full list of local switches as a map where each lot
         * is listed under the level they are in.
         * @return a map of that maps level names to maps that map switch names
         *         to their values.
         */
        std::unordered_map<std::string, std::unordered_map<std::string, int>>
            const &getLocalSwitches() const;

    private:
        int id;
        std::unordered_map<std::string, int> items;
        std::unordered_map<std::string, int> switches;
        std::unordered_map<std::string, std::unordered_map<std::string, int>>
            localSwitches;
};

#endif
