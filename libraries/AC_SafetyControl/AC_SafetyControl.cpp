/*
 * AC_SafetyControl.cpp
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#include "AC_SafetyControl.h"
#include "AC_Behaviour_Battery_Monitor.h"

AC_SafetyControl* AC_SafetyControl::module = 0;

AC_SafetyControl::AC_SafetyControl() {
    behaviours.push_back(new AC_Behaviour_Battery_Monitor());
    active = false;
}

AC_SafetyControl::~AC_SafetyControl() {
    behaviours.clearAndDelete();
}

void AC_SafetyControl::init(void) {
}

bool AC_SafetyControl::isActive(void) {
    return active;
}

AC_SafetyControl* AC_SafetyControl::getInstance(void) {
    if (!AC_SafetyControl::module) {
        AC_SafetyControl::module = new AC_SafetyControl();
    }
    return AC_SafetyControl::module;
}

void AC_SafetyControl::run(void) {
    active = false;
    AMW_List<AC_Behaviour*>::Iterator* iterator = behaviours.iterator();
    while (iterator->hasNext()) {
        AC_Behaviour* behaviour = iterator->next();
        if (behaviour->isActive()) {
            if (behaviour->perform()) {
                active = true;
                break;
            }
        }
    }
    delete iterator;
}
