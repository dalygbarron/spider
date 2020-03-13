#ifndef MEMORY_H
#define MEMORY_H

/**
 * Stores the state of stuff that is remembered long term as in stuff the
 * player has done etc.
 */
class Memory {
    public:
        /**
         * Creates the memory object.
         * @param nItems is the number of possible items that there are in the
         *               so that the inventory can be correctly instantiated.
         */
        Memory(int nItems);

        /**
         * Destroys the item count array.
         */
        ~Memory();

        /**
         * Gives you the value of a switch.
         * @param name is the name of the switch to check the value of.
         * @return the value of the switch. If the given switch has never been
         * specified then it's value is considered to be false.
         */
        int getSwitch(char const *name) const;

        /**
         * Sets the value of a switch.
         * @param name is the name of the switch to set.
         * @param value is the value to set it to.
         */
        void setSwitch(char const *name, int value);



    private:
        int id;
        unordered_map<std::string, int> switches;
        int *inventory;
};

#endif
