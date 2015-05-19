/*
 * AMW_List.h
 *
 *  Created on: 31-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_LIST_H_
#define AMW_LIST_H_

#include <stdint.h>

template <class T>
class AMW_List {
public:

    AMW_List() {
        head = 0;
        tail = 0;
        length = 0;
    }

    virtual ~AMW_List() {
        clear();
    }

    virtual bool empty(void) {
        return (head == 0);
    }

    virtual uint32_t size(void) {
        return length;
    }

    virtual T front(void) {
        if (empty())
            return 0;
        return head->item;
    }

    virtual T back(void) {
        if (empty())
            return 0;
        return tail->item;
    }

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
        }
    }

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
            delete item;
        }
    }
    virtual void eraseAndDelete(uint32_t position) {
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
            delete item->item;
            delete item;
        }
    }

    virtual T get(uint32_t position) {
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

    virtual void clearAndDelete(void) {
        ListItem* item;
        while (head != 0) {
            item = head;
            head = item->next;
            delete item->item;
            delete item;
        }
    }
    virtual void clear(void) {
        ListItem* item;
        while (head != 0) {
            item = head;
            head = item->next;
            delete item;
        }
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

        bool hasNext() {
            if (currentList->empty())
                return false;
            if (currentItem == 0)
                return true;
            else
                return (currentItem->next != 0);
        }

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
        Iterator(AMW_List<T>* list) : currentList(list), currentItem(0) { }
    private:
        AMW_List<T>* currentList;
        ListItem* currentItem;

    };

    virtual AMW_List<T>::Iterator* iterator(void) {
        return new AMW_List<T>::Iterator(this);
    }


private:
    ListItem* head;
    ListItem* tail;
    uint32_t length;

};


#endif /* AMW_LIST_H_ */
