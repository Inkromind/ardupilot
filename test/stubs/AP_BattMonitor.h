/*
 * AP_BattMonitor.h
 *
 *  Created on: 8-jul.-2015
 *      Author: Arne
 */

#ifndef STUBS_AP_BATTMONITOR_H_
#define STUBS_AP_BATTMONITOR_H_

#include <stdint.h>

class AP_BattMonitor
{
public:

    /// capacity_remaining_pct - returns the % battery capacity remaining (0 ~ 100)
    virtual uint8_t capacity_remaining_pct() const { return 0; }
    virtual ~AP_BattMonitor(void) {};
};


#endif /* STUBS_AP_BATTMONITOR_H_ */
