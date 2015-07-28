/*
 * ListTest.cpp
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

//CppUTest
#include "CppUTest/TestHarness.h"

//Class being tested
#include "../../libraries/AMW_List/AMW_List.h"

class DummyClass {
public:
    DummyClass() {}
};

AMW_List<DummyClass*>* list = 0;

TEST_GROUP(List)
{

    void setup()
    {
        list = new AMW_List<DummyClass*>();
    }
    void teardown()
    {
        delete list;
        list = 0;
    }
};

TEST(List, PushFront) {
    DummyClass dummy1;
    DummyClass dummy2;
    list->push_front(&dummy1);
    list->push_front(&dummy2);

    CHECK_EQUAL(2, list->size());
    CHECK_EQUAL(&dummy1, list->back());
    CHECK_EQUAL(&dummy2, list->front());

}

TEST(List, PushBack) {
    DummyClass dummy1;
    DummyClass dummy2;
    list->push_back(&dummy1);
    list->push_back(&dummy2);

    CHECK_EQUAL(2, list->size());
    CHECK_EQUAL(&dummy2, list->back());
    CHECK_EQUAL(&dummy1, list->front());
}

TEST(List, PopFront) {
    DummyClass dummy1;
    DummyClass dummy2;
    list->push_front(&dummy1);
    list->push_front(&dummy2);
    CHECK_EQUAL(2, list->size());

    list->pop_front();
    CHECK_EQUAL(1, list->size());
    CHECK_EQUAL(&dummy1, list->front());
    CHECK_EQUAL(&dummy1, list->back());

    list->pop_front();
    CHECK_TRUE(list->empty());

    list->pop_front();

    list->push_front(&dummy1);
    CHECK_EQUAL(1, list->size());
    CHECK_EQUAL(&dummy1, list->front());
    CHECK_EQUAL(&dummy1, list->back());
}

TEST(List, PopBack) {
    DummyClass dummy1;
    DummyClass dummy2;
    list->push_front(&dummy1);
    list->push_front(&dummy2);
    CHECK_EQUAL(2, list->size());

    list->pop_back();
    CHECK_EQUAL(1, list->size());
    CHECK_EQUAL(&dummy2, list->front());
    CHECK_EQUAL(&dummy2, list->back());

    list->pop_back();
    CHECK_TRUE(list->empty());

    list->pop_back();

    list->push_front(&dummy1);
    CHECK_EQUAL(1, list->size());
    CHECK_EQUAL(&dummy1, list->front());
    CHECK_EQUAL(&dummy1, list->back());
}

TEST(List, EraseTooBig) {
    DummyClass dummy1;
    list->push_front(&dummy1);
    list->erase(1);
    CHECK_EQUAL(1, list->size());
    CHECK_EQUAL(&dummy1, list->front());
    CHECK_EQUAL(&dummy1, list->back());
}

TEST(List, EraseFront) {
    DummyClass dummy1;
    DummyClass dummy2;
    list->push_front(&dummy2);
    list->push_front(&dummy1);
    list->erase(0);
    CHECK_EQUAL(1, list->size());
    CHECK_EQUAL(&dummy2, list->front());
    CHECK_EQUAL(&dummy2, list->back());
}

TEST(List, EraseBack) {
    DummyClass dummy1;
    DummyClass dummy2;
    list->push_front(&dummy2);
    list->push_front(&dummy1);
    list->erase(1);
    CHECK_EQUAL(1, list->size());
    CHECK_EQUAL(&dummy1, list->front());
    CHECK_EQUAL(&dummy1, list->back());
}

TEST(List, EraseMiddle) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    list->push_front(&dummy3);
    list->push_front(&dummy2);
    list->push_front(&dummy1);
    list->erase(1);
    CHECK_EQUAL(2, list->size());
    CHECK_EQUAL(&dummy1, list->front());
    CHECK_EQUAL(&dummy3, list->back());
}

TEST(List, InsertPositionTooBig) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    list->push_front(&dummy1);
    list->push_front(&dummy2);
    list->insert(5, &dummy3);
    CHECK_EQUAL(3, list->size());
    CHECK_EQUAL(&dummy3, list->back());
    CHECK_EQUAL(&dummy2, list->front());
    list->pop_front();
    CHECK_EQUAL(&dummy1, list->front());
}

TEST(List, InsertBack) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    list->push_front(&dummy1);
    list->push_front(&dummy2);
    list->insert(2, &dummy3);
    CHECK_EQUAL(3, list->size());
    CHECK_EQUAL(&dummy3, list->back());
    CHECK_EQUAL(&dummy2, list->front());
    list->pop_front();
    CHECK_EQUAL(&dummy1, list->front());
    list->pop_back();
    CHECK_EQUAL(&dummy1, list->back());
}

TEST(List, InsertFront) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    list->push_front(&dummy1);
    list->push_front(&dummy2);
    list->insert(0, &dummy3);
    CHECK_EQUAL(3, list->size());
    CHECK_EQUAL(&dummy3, list->front());
    CHECK_EQUAL(&dummy1, list->back());
    list->pop_front();
    CHECK_EQUAL(&dummy2, list->front());
}

TEST(List, InsertMiddle) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    list->push_front(&dummy1);
    list->push_front(&dummy2);
    list->insert(1, &dummy3);
    CHECK_EQUAL(3, list->size());
    CHECK_EQUAL(&dummy2, list->front());
    CHECK_EQUAL(&dummy1, list->back());
    list->pop_front();
    CHECK_EQUAL(&dummy3, list->front());
    list->pop_front();
    CHECK_EQUAL(&dummy1, list->front());
}

TEST(List, GetEmpty) {
    CHECK_EQUAL(0, list->get(0));
}

TEST(List, GetPositionTooBig) {
    DummyClass dummy1;
    list->push_front(&dummy1);
    CHECK_EQUAL(0, list->get(1));
}

TEST(List, GetFront) {
    DummyClass dummy1;
    DummyClass dummy2;
    list->push_front(&dummy1);
    list->push_front(&dummy2);
    CHECK_EQUAL(&dummy2, list->get(0));
}

TEST(List, GetBack) {
    DummyClass dummy1;
    DummyClass dummy2;
    list->push_front(&dummy1);
    list->push_front(&dummy2);
    CHECK_EQUAL(&dummy1, list->get(1));
}

TEST(List, GetMiddle) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    list->push_front(&dummy1);
    list->push_front(&dummy2);
    list->push_front(&dummy3);
    CHECK_EQUAL(&dummy2, list->get(1));
}

TEST(List, ClearEmpty) {
    list->clear();
    CHECK_TRUE(list->empty());

    DummyClass dummy1;
    list->push_front(&dummy1);

    CHECK_FALSE(list->empty());
    CHECK_EQUAL(1, list->size());
    CHECK_EQUAL(&dummy1, list->front());
    CHECK_EQUAL(&dummy1, list->back());
}

TEST(List, ClearNotEmpty) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;

    list->push_front(&dummy1);
    list->push_front(&dummy2);
    list->push_front(&dummy3);
    list->clear();
    CHECK_TRUE(list->empty());

    list->push_front(&dummy1);
    CHECK_FALSE(list->empty());
    CHECK_EQUAL(1, list->size());
    CHECK_EQUAL(&dummy1, list->front());
    CHECK_EQUAL(&dummy1, list->back());
}

TEST(List, IteratorEmpty) {
    AMW_List<DummyClass*>::Iterator* iterator = list->iterator();

    CHECK_FALSE(iterator->hasNext());
    CHECK_EQUAL(0, iterator->next());

    delete iterator;
}

TEST(List, IteratorNotEmpty) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;

    list->push_front(&dummy1);
    list->push_front(&dummy2);
    list->push_front(&dummy3);

    AMW_List<DummyClass*>::Iterator* iterator = list->iterator();

    CHECK_TRUE(iterator->hasNext());
    CHECK_EQUAL(&dummy3, iterator->next());
    CHECK_TRUE(iterator->hasNext());
    CHECK_EQUAL(&dummy2, iterator->next());
    CHECK_TRUE(iterator->hasNext());
    CHECK_EQUAL(&dummy1, iterator->next());
    CHECK_FALSE(iterator->hasNext());
    CHECK_EQUAL(0, iterator->next());
    delete iterator;

    CHECK_EQUAL(3, list->size());
    CHECK_EQUAL(&dummy3, list->front());
    CHECK_EQUAL(&dummy1, list->back());
    list->pop_front();
    CHECK_EQUAL(&dummy2, list->front());
    list->pop_front();
    CHECK_EQUAL(&dummy1, list->front());
}
