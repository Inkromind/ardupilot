/*
 * MAD_Task_Delay.cpp
 *
 *  Created on: 20-feb.-2015
 *      Author: Arne
 */

#include "MAD_Task_Delay.h"

MAD_Task_Delay::MAD_Task_Delay() {
    // TODO Auto-generated constructor stub
}

MAD_Task_Delay::~MAD_Task_Delay() {
    // TODO Auto-generated destructor stub
}

void MAD_Task_Delay::runTask() {
    if (this->running)
        return;

    // TODO
}

bool MAD_Task_Delay::isComplete() {
    if (this->completed)
        return true;
    if (!this->running)
        return false;

    // TODO
    return false;
}

