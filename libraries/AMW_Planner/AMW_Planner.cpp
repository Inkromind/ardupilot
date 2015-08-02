/*
 * AMW_Planner.cpp
 *
 *  Created on: 13-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Planner.h"
#include "AMW_Sequencer.h"
#include <AC_Facade.h>
#include <stdlib.h>

bool AMW_Planner::previousToggleState = true;
bool AMW_Planner::initialized = false;
AMW_Module_Identifier* AMW_Planner::moduleIdentifier = new AMW_Module_Identifier(0);

void AMW_Planner::initPlanner() {
    if (initialized)
        return;
    srand(AC_Facade::getFacade()->getTimeMillis());
    if (AC_Facade::getFacade()->initFlightMode()) {
        AMW_Task_Planner::getInstance()->init();
        AMW_Sequencer::getInstance()->init();
        initialized = true;
    }
}


void AMW_Planner::run100Hz() {

}

void AMW_Planner::run50Hz() {

}

void AMW_Planner::run10Hz() {
    AMW_Task_Planner::getInstance()->run();
    AMW_Sequencer::getInstance()->run();
}

void AMW_Planner::run1Hz() {
    AMW_Planner::checkMissionToggle();
}

void AMW_Planner::pauseMission() {
    AMW_Sequencer::getInstance()->pauseMission();
}

void AMW_Planner::resumeMission() {
    if (AC_Facade::getFacade()->initFlightMode()) {
        AMW_Task_Planner::getInstance()->resumeMission();
        AMW_Sequencer::getInstance()->resumeMission();
    }
}

void AMW_Planner::toggleMission() {
    if (!initialized)
        initPlanner();
    else {
        AMW_Sequencer::getInstance()->toggleMission();
    }
}


void AMW_Planner::checkMissionToggle() {
    if (AC_Facade::getFacade()->getCH8Position()) {
        if (!AMW_Planner::previousToggleState)
            AMW_Planner::toggleMission();
        AMW_Planner::previousToggleState = true;
    }
    else
        AMW_Planner::previousToggleState = false;
}

void AMW_Planner::returnHome() {
    if (AC_Facade::getFacade()->initFlightMode()) {
        AMW_Task_Planner::getInstance()->returnHome();
        AMW_Sequencer::getInstance()->resumeMission();
    }
}

bool AMW_Planner::isBatteryEmpty() {
    return AMW_Task_Planner::getInstance()->isBatteryEmpty();
}

void AMW_Planner::markBatteryEmpty() {
    AMW_Task_Planner::getInstance()->markBatteryEmpty();
}

void AMW_Planner::setHomebase(Vector2f newHomebase) {
    AMW_Task_Planner::getInstance()->setHomeBase(newHomebase);
}
