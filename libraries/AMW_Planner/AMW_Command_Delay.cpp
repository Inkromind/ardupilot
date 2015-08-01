/*
 *AMW_Command_Delay.cpp
 *
 *  Created on: 20-feb.-2015
 *      Author: Arne
 */

#include "AMW_Command_Delay.h"
#include <AC_CommunicationFacade.h>
#include <AC_Facade.h>


AMW_Command_Delay::AMW_Command_Delay(uint32_t delay) : AMW_Command() {
    this->duration = (uint32_t)max(delay, 0);
    this->start = 0;
}

void AMW_Command_Delay::run(bool attempt) {
    updateStatus();

    if (completed)
        return;

    AC_Facade::getFacade()->loiter();

    if (commandStarted)
        return;

    this->start = AC_Facade::getFacade()->getTimeMillis();
    this->commandStarted = true;
#ifdef AMW_COMMAND_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Starting delay"));
#endif
}

void AMW_Command_Delay::updateStatus() {
    if (completed)
           return;
    if (!commandStarted)
           return;

    if (AC_Facade::getFacade()->getTimeMillis() - start > duration) {
        completed = true;
#ifdef AMW_COMMAND_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("Delay Completed"));
#endif
    }
}

void AMW_Command_Delay::pause() {
    this->duration -= AC_Facade::getFacade()->getTimeMillis() - start;
    start = 0;
}

void AMW_Command_Delay::resume() {
    this->start = AC_Facade::getFacade()->getTimeMillis();
}
