/*
 * AMW_Task_Land.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Task_Land.h"
#include <AC_Facade.h>
#include <AMW_Task_Land_State_Start.h>

AMW_Task_Land::AMW_Task_Land() {
    currentState = new AMW_Task_Land_State_Start(this);
}
