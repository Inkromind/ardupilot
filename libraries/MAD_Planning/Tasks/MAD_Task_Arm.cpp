/*
 * MAD_Task_Arm.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "MAD_Task_Arm.h"

MAD_Task_Arm::MAD_Task_Arm() {
}

MAD_Task_Arm::~MAD_Task_Arm() {
}

void MAD_Task_Arm::runTask() {
    if (this->running)
        return;

    if (MAD_Facade::armMotors())
        this->running = true;
}

bool MAD_Task_Arm::isComplete() {
    if (this->completed)
        return true;
    if (!this->running)
        return false;

    if (MAD_Facade::areMotorsArmed()) {
        this->completed = true;
        return true;
    }
    return false;
}
