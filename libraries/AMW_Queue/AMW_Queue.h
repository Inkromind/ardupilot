/*
 * AMW_Queue.h
 *
 *  Created on: 13-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_QUEUE_H_
#define AMW_QUEUE_H_

#include <stdint.h>

template <class T>
class AMW_Queue {

public:
    AMW_Queue() {
        head = 0;
        tail = 0;
        length = 0;
    }
    virtual ~AMW_Queue() {
        while (head != 0) {
            pop();
        };
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
