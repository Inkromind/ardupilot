/*
 * AMW_Command_Land.cpp
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Command_Land.h"
#include <AC_Facade.h>

AMW_Command_Land::AMW_Command_Land() { }

void AMW_Command_Land::runCommand() {
    updateStatus();

    if (completed)
        return;

    AC_Facade::land();
}

void AMW_Command_Land::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::isLanded()) {
        completed = true;
    }
}
