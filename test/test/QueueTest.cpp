/*
 * QueueTest.cpp
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

//CppUTest
#include "CppUTest/TestHarness.h"

//Class being tested
#include "../../libraries/AMW_Queue/AMW_Queue.h"

class DummyClass {
public:
    DummyClass() {}
};

AMW_Queue<DummyClass*>* queue = 0;

TEST_GROUP(Queue)
{

    void setup()
    {
        queue = new AMW_Queue<DummyClass*>();
    }
    void teardown()
    {
        delete queue;
        queue = 0;
    }
};

TEST(Queue, Push) {
    DummyClass dummy1;
    DummyClass dummy2;
    queue->push(&dummy1);
    queue->push(&dummy2);

    CHECK_EQUAL(2, queue->size());
    CHECK_EQUAL(&dummy2, queue->back());
    CHECK_EQUAL(&dummy1, queue->front());

}

TEST(Queue, Pop) {
    DummyClass dummy1;
    DummyClass dummy2;
    queue->push(&dummy1);
    queue->push(&dummy2);
    CHECK_EQUAL(2, queue->size());

    queue->pop();
    CHECK_EQUAL(1, queue->size());
    CHECK_EQUAL(&dummy2, queue->front());
    CHECK_EQUAL(&dummy2, queue->back());

    queue->pop();
    CHECK_TRUE(queue->empty());

    queue->pop();

    queue->push(&dummy1);
    CHECK_EQUAL(1, queue->size());
    CHECK_EQUAL(&dummy1, queue->front());
    CHECK_EQUAL(&dummy1, queue->back());
}

TEST(Queue, ClearEmpty) {
    queue->clear();
    CHECK_TRUE(queue->empty());

    DummyClass dummy1;
    queue->push(&dummy1);

    CHECK_FALSE(queue->empty());
    CHECK_EQUAL(1, queue->size());
    CHECK_EQUAL(&dummy1, queue->front());
    CHECK_EQUAL(&dummy1, queue->back());
}

TEST(Queue, ClearNotEmpty) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;

    queue->push(&dummy1);
    queue->push(&dummy2);
    queue->push(&dummy3);
    queue->clear();
    CHECK_TRUE(queue->empty());

    queue->push(&dummy1);
    CHECK_FALSE(queue->empty());
    CHECK_EQUAL(1, queue->size());
    CHECK_EQUAL(&dummy1, queue->front());
    CHECK_EQUAL(&dummy1, queue->back());
}
