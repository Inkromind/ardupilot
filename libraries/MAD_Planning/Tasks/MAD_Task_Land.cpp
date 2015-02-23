/*
 * MAD_Task_Land.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "MAD_Task_Land.h"

MAD_Task_Land::MAD_Task_Land() {
}

MAD_Task_Land::~MAD_Task_Land() {
}

void MAD_Task_Land::runTask() {
    if (this->running)
        return;

    if (MAD_Facade::startLand())
        this->running = true;
}

bool MAD_Task_Land::isComplete() {
    if (this->completed)
        return true;
    if (!this->running)
        return false;

    if (MAD_Facade::isLanded()) {
        this->completed = true;
        return true;
    }
    return false;
}
