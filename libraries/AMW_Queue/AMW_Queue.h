/*
 * AMW_Queue.h
 *
 *  Created on: 13-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_QUEUE_H_
#define AMW_QUEUE_H_

#include <stdint.h>

/**
 * A generic FIFO queue class
 */
template <class T>
class AMW_Queue {

public:
    AMW_Queue() {
        head = 0;
        tail = 0;
        length = 0;
    }

    /**
     * Delete the queue.
     * Note: Elements will not be deleted!
     */
    virtual ~AMW_Queue() {
        while (head != 0) {
            pop();
        };
    }

    /**
     * Returns if the queue is empty
     *
     * @return True if the queue is empty, false otherwise
     */
    virtual bool empty(void) const {
        return (head == 0);
    }

    /**
     * Returns the size of the queue
     *
     * @return The number of elements in the queue
     */
    virtual uint32_t size(void) const {
        return length;
    }

    /**
     * Returns the first element in the queue
     *
     * @return The first element in the queue or 0 if empty
     */
    virtual T front(void) const {
        if (empty())
            return 0;
        return head->item;
    }

    /**
     * Returns the last element in the queue
     *
     * @return The last element in the queue or 0 if empty
     */
    virtual T back(void) const {
        if (empty())
            return 0;
        return tail->item;
    }


    /**
     * Add an element to the back of the queue
     *
     * @param item The element to be added
     */
    virtual void push(const T item) {
        QueueItem* queueItem = new QueueItem(item);
        if (empty()) {
            head = queueItem;
        }
        else {
            tail->next = queueItem;
        }
        tail = queueItem;
        length++;
    }

    /**
     * Remove the first element from the queue
     * Note: the actual element will not be deleted
     */
    virtual void pop(void) {
        if (empty())
            return;
        QueueItem* queueItem = head;
        head = queueItem->next;
        length--;
        if (empty()) {
            tail = 0;
        }
        delete queueItem;
    }

    /**
     * Clear the queue
     * Note: the actual elements will not be deleted
     */
    virtual void clear(void) {
        QueueItem* item;
        while (head != 0) {
            item = head;
            head = item->next;
            length--;
            delete item;
        }
        tail = 0;
    }

private:
    struct QueueItem {
        T item;
        QueueItem* next;
        QueueItem(T data): item(data), next(0) {}
    };

    QueueItem* head;
    QueueItem* tail;
    uint32_t length;

};

#endif /* AMW_QUEUE_H_ */
