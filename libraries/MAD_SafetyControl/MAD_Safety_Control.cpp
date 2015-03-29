/*
 * MAD_Safety_Control.cpp
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#include "MAD_Safety_Control.h"

MAD_SafetyControl* MAD_SafetyControl::module = 0;

MAD_SafetyControl::MAD_SafetyControl() {
    behaviours = new std::list();
    active = false;
}

MAD_SafetyControl::~MAD_SafetyControl() {
    while (!behaviours->empty()) {
        delete behaviours->front();
        behaviours->pop_front();
    }
    delete behaviours;
}

void MAD_SafetyControl::init(void) {
}

bool MAD_SafetyControl::isActive(void) {
    return active;
}

MAD_SafetyControl* MAD_SafetyControl::getInstance(void) {
    if (!MAD_SafetyControl::module) {
        MAD_SafetyControl::module = new MAD_SafetyControl();
    }
    return MAD_SafetyControl::module;
}

void MAD_SafetyControl::run(void) {
    active = false;
    for(std::list<MAD_Behaviour*>::const_iterator behaviour = behaviours->begin(); behaviour != behaviours->end(); ++behaviour) {
        if (behaviour->perform()) {
            active = true;
            break;
        }
    }
}
