/*
 * BatteryMonitorMock.h
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

#ifndef MOCKS_BATTERYMONITORMOCK_H_
#define MOCKS_BATTERYMONITORMOCK_H_

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

class BattMonitorMock : public AP_BattMonitor {
    uint8_t capacity_remaining_pct() const {
        return (uint8_t) mock().actualCall("batteryCapacity").returnIntValue();
    }
};


#endif /* MOCKS_BATTERYMONITORMOCK_H_ */
