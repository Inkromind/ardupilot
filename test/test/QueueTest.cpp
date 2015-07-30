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

AMW_Queue<DummyClass*>* Qqueue = 0;

TEST_GROUP(Queue)
{

    void setup()
    {
        Qqueue = new AMW_Queue<DummyClass*>();
    }
    void teardown()
    {
        delete Qqueue;
        Qqueue = 0;
    }
};

TEST(Queue, Push) {
    DummyClass dummy1;
    DummyClass dummy2;

    Qqueue->push(&dummy1);
    Qqueue->push(&dummy2);

    CHECK_EQUAL(2, Qqueue->size());
    CHECK_EQUAL(&dummy2, Qqueue->back());
    CHECK_EQUAL(&dummy1, Qqueue->front());

}

TEST(Queue, Pop) {
    DummyClass dummy1;
    DummyClass dummy2;
    Qqueue->push(&dummy1);
    Qqueue->push(&dummy2);
    CHECK_EQUAL(2, Qqueue->size());

    Qqueue->pop();

    CHECK_EQUAL(1, Qqueue->size());
    CHECK_EQUAL(&dummy2, Qqueue->front());
    CHECK_EQUAL(&dummy2, Qqueue->back());

    Qqueue->pop();

    CHECK_TRUE(Qqueue->empty());

    Qqueue->pop();

    Qqueue->push(&dummy1);
    CHECK_EQUAL(1, Qqueue->size());
    CHECK_EQUAL(&dummy1, Qqueue->front());
    CHECK_EQUAL(&dummy1, Qqueue->back());
}

TEST(Queue, ClearEmpty) {
    Qqueue->clear();

    CHECK_TRUE(Qqueue->empty());
    DummyClass dummy1;
    Qqueue->push(&dummy1);
    CHECK_FALSE(Qqueue->empty());
    CHECK_EQUAL(1, Qqueue->size());
    CHECK_EQUAL(&dummy1, Qqueue->front());
    CHECK_EQUAL(&dummy1, Qqueue->back());
}

TEST(Queue, ClearNotEmpty) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    Qqueue->push(&dummy1);
    Qqueue->push(&dummy2);
    Qqueue->push(&dummy3);

    Qqueue->clear();

    CHECK_TRUE(Qqueue->empty());
    Qqueue->push(&dummy1);
    CHECK_FALSE(Qqueue->empty());
    CHECK_EQUAL(1, Qqueue->size());
    CHECK_EQUAL(&dummy1, Qqueue->front());
    CHECK_EQUAL(&dummy1, Qqueue->back());
}
