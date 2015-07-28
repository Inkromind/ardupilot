/*
 * AP_Hal.h
 *
 *  Created on: 7-jul.-2015
 *      Author: Arne
 */

#ifndef STUBS_AP_HAL_H_
#define STUBS_AP_HAL_H_
#ifndef __AP_HAL_H__

#include <stdint.h>

namespace AP_HAL {

    /* Toplevel pure virtual class Hal.*/
    class HAL;
    class Scheduler;
}
class AP_HAL::Scheduler {
public:
    virtual void     delay(uint16_t ms) { }
    virtual uint32_t millis() { return 0; }
    virtual uint32_t micros() { return 0; }
    virtual void     delay_microseconds(uint16_t us) {};
    virtual ~Scheduler(void) {};
    Scheduler(void){};
};

class AP_HAL::HAL {
public:
    HAL(AP_HAL::Scheduler*  _scheduler)
        :
        scheduler(_scheduler)
    {}

    HAL() {
        scheduler = new AP_HAL::Scheduler();
    }

    AP_HAL::Scheduler*  scheduler;
};


const AP_HAL::HAL& hal = AP_HAL::HAL();

#endif
#endif /* STUBS_AP_HAL_H_ */
