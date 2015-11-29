/*
 * AMW_Logging.h
 *
 *  Created on: 29-nov.-2015
 *      Author: Arne
 */

#ifndef STUBS_AMW_LOGGING_H_
#define STUBS_AMW_LOGGING_H_

#include <stdint.h>

class AMW_Logging {
public:
    static AMW_Logging* getInstance(void);

    virtual void sync(uint8_t id);

    virtual void logCompletedReservation(float level, uint8_t attempts);

    virtual void logFailedReservation(uint8_t attempts);

    virtual void logPosition(void);

    virtual void logConflict(void);

    virtual void logAssignedPackage(uint8_t packageId, float estimate);

    virtual void logCompletedPackage(uint8_t packageId);

    virtual void logFailedPackage(uint8_t packageId);

protected:
    AMW_Logging() {};

    virtual ~AMW_Logging() {};

};



#endif /* STUBS_AMW_LOGGING_H_ */
