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

AMW_List<DummyClass*>* Llist = 0;

TEST_GROUP(List)
{

    void setup()
    {
        Llist = new AMW_List<DummyClass*>();
    }
    void teardown()
    {
        delete Llist;
        Llist = 0;
    }
};

TEST(List, PushFront) {
    DummyClass dummy1;
    DummyClass dummy2;

    Llist->push_front(&dummy1);
    Llist->push_front(&dummy2);

    CHECK_EQUAL(2, Llist->size());
    CHECK_EQUAL(&dummy1, Llist->back());
    CHECK_EQUAL(&dummy2, Llist->front());

}

TEST(List, PushBack) {
    DummyClass dummy1;
    DummyClass dummy2;

    Llist->push_back(&dummy1);
    Llist->push_back(&dummy2);

    CHECK_EQUAL(2, Llist->size());
    CHECK_EQUAL(&dummy2, Llist->back());
    CHECK_EQUAL(&dummy1, Llist->front());
}

TEST(List, PopFront) {
    DummyClass dummy1;
    DummyClass dummy2;
    Llist->push_front(&dummy1);
    Llist->push_front(&dummy2);
    CHECK_EQUAL(2, Llist->size());

    Llist->pop_front();

    CHECK_EQUAL(1, Llist->size());
    CHECK_EQUAL(&dummy1, Llist->front());
    CHECK_EQUAL(&dummy1, Llist->back());

    Llist->pop_front();

    CHECK_TRUE(Llist->empty());

    Llist->pop_front();

    Llist->push_front(&dummy1);
    CHECK_EQUAL(1, Llist->size());
    CHECK_EQUAL(&dummy1, Llist->front());
    CHECK_EQUAL(&dummy1, Llist->back());
}

TEST(List, PopBack) {
    DummyClass dummy1;
    DummyClass dummy2;
    Llist->push_front(&dummy1);
    Llist->push_front(&dummy2);
    CHECK_EQUAL(2, Llist->size());

    Llist->pop_back();

    CHECK_EQUAL(1, Llist->size());
    CHECK_EQUAL(&dummy2, Llist->front());
    CHECK_EQUAL(&dummy2, Llist->back());

    Llist->pop_back();

    CHECK_TRUE(Llist->empty());

    Llist->pop_back();

    Llist->push_front(&dummy1);
    CHECK_EQUAL(1, Llist->size());
    CHECK_EQUAL(&dummy1, Llist->front());
    CHECK_EQUAL(&dummy1, Llist->back());
}

TEST(List, EraseTooBig) {
    DummyClass dummy1;
    Llist->push_front(&dummy1);

    Llist->erase(1);

    CHECK_EQUAL(1, Llist->size());
    CHECK_EQUAL(&dummy1, Llist->front());
    CHECK_EQUAL(&dummy1, Llist->back());
}

TEST(List, EraseFront) {
    DummyClass dummy1;
    DummyClass dummy2;
    Llist->push_front(&dummy2);
    Llist->push_front(&dummy1);

    Llist->erase(0);

    CHECK_EQUAL(1, Llist->size());
    CHECK_EQUAL(&dummy2, Llist->front());
    CHECK_EQUAL(&dummy2, Llist->back());
}

TEST(List, EraseBack) {
    DummyClass dummy1;
    DummyClass dummy2;
    Llist->push_front(&dummy2);
    Llist->push_front(&dummy1);

    Llist->erase(1);

    CHECK_EQUAL(1, Llist->size());
    CHECK_EQUAL(&dummy1, Llist->front());
    CHECK_EQUAL(&dummy1, Llist->back());
}

TEST(List, EraseMiddle) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    Llist->push_front(&dummy3);
    Llist->push_front(&dummy2);
    Llist->push_front(&dummy1);

    Llist->erase(1);

    CHECK_EQUAL(2, Llist->size());
    CHECK_EQUAL(&dummy1, Llist->front());
    CHECK_EQUAL(&dummy3, Llist->back());
}

TEST(List, InsertPositionTooBig) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    Llist->push_front(&dummy1);
    Llist->push_front(&dummy2);

    Llist->insert(5, &dummy3);

    CHECK_EQUAL(3, Llist->size());
    CHECK_EQUAL(&dummy3, Llist->back());
    CHECK_EQUAL(&dummy2, Llist->front());
    Llist->pop_front();
    CHECK_EQUAL(&dummy1, Llist->front());
}

TEST(List, InsertBack) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    Llist->push_front(&dummy1);
    Llist->push_front(&dummy2);

    Llist->insert(2, &dummy3);

    CHECK_EQUAL(3, Llist->size());
    CHECK_EQUAL(&dummy3, Llist->back());
    CHECK_EQUAL(&dummy2, Llist->front());
    Llist->pop_front();
    CHECK_EQUAL(&dummy1, Llist->front());
    Llist->pop_back();
    CHECK_EQUAL(&dummy1, Llist->back());
}

TEST(List, InsertFront) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    Llist->push_front(&dummy1);
    Llist->push_front(&dummy2);

    Llist->insert(0, &dummy3);

    CHECK_EQUAL(3, Llist->size());
    CHECK_EQUAL(&dummy3, Llist->front());
    CHECK_EQUAL(&dummy1, Llist->back());
    Llist->pop_front();
    CHECK_EQUAL(&dummy2, Llist->front());
}

TEST(List, InsertMiddle) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    Llist->push_front(&dummy1);
    Llist->push_front(&dummy2);

    Llist->insert(1, &dummy3);

    CHECK_EQUAL(3, Llist->size());
    CHECK_EQUAL(&dummy2, Llist->front());
    CHECK_EQUAL(&dummy1, Llist->back());
    Llist->pop_front();
    CHECK_EQUAL(&dummy3, Llist->front());
    Llist->pop_front();
    CHECK_EQUAL(&dummy1, Llist->front());
}

TEST(List, GetEmpty) {
    CHECK_EQUAL(0, Llist->get(0));
}

TEST(List, GetPositionTooBig) {
    DummyClass dummy1;
    Llist->push_front(&dummy1);

    CHECK_EQUAL(0, Llist->get(1));
}

TEST(List, GetFront) {
    DummyClass dummy1;
    DummyClass dummy2;
    Llist->push_front(&dummy1);
    Llist->push_front(&dummy2);

    CHECK_EQUAL(&dummy2, Llist->get(0));
}

TEST(List, GetBack) {
    DummyClass dummy1;
    DummyClass dummy2;
    Llist->push_front(&dummy1);
    Llist->push_front(&dummy2);

    CHECK_EQUAL(&dummy1, Llist->get(1));
}

TEST(List, GetMiddle) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    Llist->push_front(&dummy1);
    Llist->push_front(&dummy2);
    Llist->push_front(&dummy3);

    CHECK_EQUAL(&dummy2, Llist->get(1));
}

TEST(List, ClearEmpty) {
    Llist->clear();

    CHECK_TRUE(Llist->empty());
    DummyClass dummy1;
    Llist->push_front(&dummy1);
    CHECK_FALSE(Llist->empty());
    CHECK_EQUAL(1, Llist->size());
    CHECK_EQUAL(&dummy1, Llist->front());
    CHECK_EQUAL(&dummy1, Llist->back());
}

TEST(List, ClearNotEmpty) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    Llist->push_front(&dummy1);
    Llist->push_front(&dummy2);
    Llist->push_front(&dummy3);

    Llist->clear();

    CHECK_TRUE(Llist->empty());
    Llist->push_front(&dummy1);
    CHECK_FALSE(Llist->empty());
    CHECK_EQUAL(1, Llist->size());
    CHECK_EQUAL(&dummy1, Llist->front());
    CHECK_EQUAL(&dummy1, Llist->back());
}

TEST(List, IteratorEmpty) {
    AMW_List<DummyClass*>::Iterator* iterator = Llist->iterator();

    CHECK_FALSE(iterator->hasNext());
    CHECK_EQUAL(0, iterator->next());

    delete iterator;
}

TEST(List, IteratorNotEmpty) {
    DummyClass dummy1;
    DummyClass dummy2;
    DummyClass dummy3;
    Llist->push_front(&dummy1);
    Llist->push_front(&dummy2);
    Llist->push_front(&dummy3);

    AMW_List<DummyClass*>::Iterator* iterator = Llist->iterator();

    CHECK_TRUE(iterator->hasNext());
    CHECK_EQUAL(&dummy3, iterator->next());
    CHECK_TRUE(iterator->hasNext());
    CHECK_EQUAL(&dummy2, iterator->next());
    CHECK_TRUE(iterator->hasNext());
    CHECK_EQUAL(&dummy1, iterator->next());
    CHECK_FALSE(iterator->hasNext());
    CHECK_EQUAL(0, iterator->next());
    delete iterator;

    CHECK_EQUAL(3, Llist->size());
    CHECK_EQUAL(&dummy3, Llist->front());
    CHECK_EQUAL(&dummy1, Llist->back());
    Llist->pop_front();
    CHECK_EQUAL(&dummy2, Llist->front());
    Llist->pop_front();
    CHECK_EQUAL(&dummy1, Llist->front());
}
