#ifndef POOL_H
#define POOL_H

/**
 * A pool of things that can be reused. Each time that they are reused, their
 * id number is incrememnted by the maximum size of the pool so that if you
 * come back after a while and something else has take the spot of the item you
 * had the id of you will know your one is gone.
 */
template <class T> class Pool {
    public:
        /**
         * Represents an item in the pool.
         */
        class Item {
            public:
                // TODO: consider making some of this stuff private and making
                //       pool a friend class.
                unsigned int id;
                int alive;
                union Content {
                    T live;
                    Item *next;

                    /**
                     * Default constructor for the content union.
                     */
                    Content() {
                        this->next = NULL;
                    }
                } content;
        };

    private:
        unsigned int size;
        std::vector<Pool<T>::Item> items;
        Pool<T>::Item *firstFree;

    public:
        /**
         * Creates an empty pool of given size.
         * @param size is the number of things it can hold at once maximum.
         */
        Pool<T>(unsigned int size) {
            this->size = size;
            this->items.resize(size);
            this->firstFree = this->items.data();
            for (int i = 0; i < size; i++) {
                Pool<T>::Item item;
                item.id = i;
                item.alive = false;
                item.content.next = this->items.data() + i + 1;
                this->items[i] = item;
            }
            this->items[size - 1].content.next = NULL;
        }

        /**
         * Finds the item with the given id in the pool and returns it if it
         * exists.
         * @param id is the id to look for.
         * @return a pointer to the item if found or null.
         */
        Pool<T>::Item *get(unsigned int id) {
            T *item = &this->items[id % this->size];
            if (item->id == id) return item;
            return NULL;
        }

        /**
         * Adds somethign to the pool.
         * @param item is the thing to add.
         * @return a pointer to the item in the pool if it was added, and null
         *         if the pool is closed due to aids.
         */
        Pool<T>::Item *add(T item) {
            if (!this->firstFree) return NULL;
            Pool<T>::Item *oldFirst = this->firstFree;
            this->firstFree = this->firstFree->content.next;
            oldFirst->content.live = std::move(item);
            oldFirst->alive = true;
            return oldFirst;
        }

        /**
         * Remove an item from the pool, set it as dead and add it to the dead
         * free list.
         * @param id is the id of the thingo to remove.
         */
        void remove(unsigned int id) {
            Pool<T>::Item *item = &this->items[id % this->size];
            if (item->id != id) return;
            item->alive = false;
            item->content.next = this->firstFree;
            this->firstFree = this->items.data() + (id % this->size);
        }

        /**
         * Gives you read only access to all of the items in the pool
         * @return a reference tothe pool.
         */
        std::vector<Pool<T>::Item> const &getItems() const {
            return this->items;
        }

        /**
         * Gives you access to all of the items in the pool
         * @return a mutable reference tothe pool.
         */
        std::vector<Pool<T>::Item> &getItemsMutable() {
            return this->items;
        }
};

#endif
