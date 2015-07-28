/*
 * CorridorComparator.h
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

#ifndef MOCKS_CORRIDORCOMPARATOR_H_
#define MOCKS_CORRIDORCOMPARATOR_H_

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
class CorridorComparator : public MockNamedValueComparator
{
public:
    virtual bool isEqual(void* object1, void* object2)
    {
        return object1 == object2;
    }
    virtual SimpleString valueToString(void* object)
    {
        return StringFrom(object);
    }
};

#endif /* MOCKS_CORRIDORCOMPARATOR_H_ */
