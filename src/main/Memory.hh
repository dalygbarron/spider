#ifndef MEMORY_H
#define MEMORY_H

/**
 * Stores the state of stuff that is remembered long term as in stuff the
 * player has done etc.
 */
class Memory {
    public:
        int const id;

        /**
         * Creates a new memory.
         * @param id is the id of the memory to make.
         */
        Memory(int id);

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
         * Gives you the number that the player has of a given item.
         * @param item is the item to get the number of.
         * @return the number.
         */
        int getItemCount(Item const &item) const;

        /**
         * Sets the number that the player has of a given item.
         * @param item  is to set the count of.
         * @param count is the number to set it to.
         */
        void setItemCount(Item const &item, int count);

        /**
         * Gives you read only access to the full switch list.
         * @return the switch list.
         */
        unordered_map<std::string, int> const &getSwitches() const;

        /**
         * Gives you read only access to the full item list.
         * @return the item list.
         */
        unordered_map<std::string, int> const &getItems() const;

    private:
        unordered_map<std::string, int> switches;
        unordered_map<std::string, int> items;
};

#endif
