/*
 * MAD_Task_Nav.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "MAD_Task_Nav.h"
#include "AC_Facade.h"

MAD_Task_Nav::MAD_Task_Nav(Vector3f destination) {
    this->destination = destination;
}

MAD_Task_Nav::~MAD_Task_Nav() {
    delete destination;
}

void MAD_Task_Nav::runTask() {
    if (this->running)
        return;

    if (AC_Facade::startNav(this->destination))
        this->running = true;
}

bool MAD_Task_Nav::isComplete() {
    if (this->completed)
        return true;
    if (!this->running)
        return false;

    if (AC_Facade::destinationReached(this->destination)) {
        this->completed = true;
        return true;
    }
    return false;
}
