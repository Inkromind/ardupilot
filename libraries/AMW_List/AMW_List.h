/*
 * AMW_List.h
 *
 *  Created on: 31-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_LIST_H_
#define AMW_LIST_H_

#include <stdint.h>

/**
 * A generic list class
 */
template <class T>
class AMW_List {
public:

    /**
     * Initialize a list
     */
    AMW_List() {
        head = 0;
        tail = 0;
        length = 0;
    }

    /**
     * Delete the list.
     * Note: Elements will not be deleted!
     */
    virtual ~AMW_List() {
        clear();
    }

    /**
     * Returns if the list is empty
     *
     * @return True if the list is empty, false otherwise
     */
    virtual bool empty(void) const {
        return (head == 0);
    }

    /**
     * Returns the size of the list
     *
     * @return The number of elements in the list
     */
    virtual uint32_t size(void) const {
        return length;
    }

    /**
     * Returns the first element in the list
     *
     * @return The first element in the list or 0 if empty
     */
    virtual T front(void) const {
        if (empty())
            return 0;
        return head->item;
    }

    /**
     * Returns the last element in the list
     *
     * @return The last element in the list or 0 if empty
     */
    virtual T back(void) const {
        if (empty())
            return 0;
        return tail->item;
    }

    /**
     * Add an element to the front of the list
     *
     * @param item The element to be added
     */
    virtual void push_front(const T item) {
        ListItem* listItem = new ListItem(item);
        if (!empty()) {
            listItem->next = head;
            head->prev = listItem;
        }
        else {
            tail = listItem;
        }
        head = listItem;
        length++;
    }

    /**
     * Add an element to the back of the list
     *
     * @param item The element to be added
     */
    virtual void push_back(const T item) {
        ListItem* listItem = new ListItem(item);
        if (!empty()) {
            listItem->prev = tail;
            tail->next = listItem;
        }
        else {
            head = listItem;
        }
        tail = listItem;
        length++;
    }

    /**
     * Remove the first element from the list
     * Note: the actual element will not be deleted
     */
    virtual void pop_front(void) {
        if (empty())
            return;
        ListItem* listItem = head;
        head = listItem->next;
        length--;
        if (empty()) {
            tail = 0;
        }
        else {
            head->prev = 0;
        }
        delete listItem;
    }

    /**
     * Remove the last element from the list
     * Note: the actual element will not be deleted
     */
    virtual void pop_back(void) {
        if (empty())
            return;
        ListItem* listItem = tail;
        tail = listItem->prev;
        length--;
        if (length == 0) {
            head = 0;
        }
        else {
            tail->next = 0;
        }
        delete listItem;
    }

    /**
     * Insert an element in the list at the given position.
     * If the given position is greater than the size of the list,
     * the element will be added at the back of the list
     *
     * @param position The position to add the element at
     * @param item The element to add
     */
    virtual void insert(uint32_t position, T item) {
        if (position >= size())
            push_back(item);
        else if (position == 0)
            push_front(item);
        else {
            ListItem* listItem = new ListItem(item);
            ListItem* nextItem = head;
            for (uint32_t var = 0; var < position; ++var) {
                nextItem = nextItem->next;
            }
            listItem->prev = nextItem->prev;
            listItem->next = nextItem;
            nextItem->prev->next = listItem;
            nextItem->prev = listItem;
            length++;
        }
    }

    /**
     * Remove the element at the given position from the list
     * Note: the actual element will not be deleted
     *
     * @param position The position of the element to remove
     */
    virtual void erase(uint32_t position) {
        if (position >= size())
            return;
        else if (position == size() - 1)
            pop_back();
        else if (position == 0)
            pop_front();
        else {
            ListItem* item = head;
            for (uint32_t var = 0; var < position; ++var) {
                item = item->next;
            }
            item->prev->next = item->next;
            item->next->prev = item->prev;
            length--;
            delete item;
        }
    }

    /**
     * Get the element at the given position
     *
     * @param position The position of the element to get
     * @return The item at the given position or 0 if invalid position
     */
    virtual T get(uint32_t position) const {
        if (position >= size())
            return 0;
        else if (position == size() - 1)
            return tail->item;
        else if (position == 0)
            return head->item;
        else {
            ListItem* item = head;
            for (uint32_t var = 0; var < position; ++var) {
                item = item->next;
            }
            return item->item;
        }
    }

    /**
     * Clear the list
     * Note: the actual elements will not be deleted
     */
    virtual void clear(void) {
        ListItem* item;
        while (head != 0) {
            item = head;
            head = item->next;
            length--;
            delete item;
        }
        tail = 0;
    }

    struct ListItem {
        T item;
        ListItem* next;
        ListItem* prev;
        ListItem(T data): item(data), next(0), prev(0) {}
    };

    class Iterator {
    public:
        ~Iterator() {}

        /**
         * Returns if there is still another element in the iterator to return
         */
        bool hasNext() {
            if (currentList->empty())
                return false;
            if (currentItem == 0)
                return true;
            else
                return (currentItem->next != 0);
        }

        /**
         * Returns the next element
         */
        T next() {
            if (hasNext()) {
                if (currentItem == 0)
                    currentItem = currentList->head;
                else
                    currentItem = currentItem->next;
                return currentItem->item;
            }
            else
                return 0;
        }
        Iterator(const AMW_List<T>* list) : currentList(list), currentItem(0) { }
    private:
        const AMW_List<T>* currentList;
        const ListItem* currentItem;

    };

    /**
     * Returns an iterator for the list
     */
    virtual AMW_List<T>::Iterator* iterator(void) const {
        return new AMW_List<T>::Iterator(this);
    }


private:
    ListItem* head;
    ListItem* tail;
    uint32_t length;

};


#endif /* AMW_LIST_H_ */
