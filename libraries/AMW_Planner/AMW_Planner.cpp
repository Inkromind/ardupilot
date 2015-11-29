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
float AMW_Planner::distanceTravelled = 0;
Vector3f AMW_Planner::lastPosition = Vector3f();

void AMW_Planner::initPlanner() {
    if (initialized)
        return;
    distanceTravelled = 0;
    lastPosition = AC_Facade::getFacade()->getRealPosition();
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
    if (initialized) {
        Vector3f newPosition = AC_Facade::getFacade()->getRealPosition();
        if (!AC_Facade::getFacade()->isLanded())
            distanceTravelled += (lastPosition - newPosition).length();
        lastPosition = newPosition;
    }
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

AMW_Planner_Counters_t AMW_Planner::getCounters() {
    AMW_Planner_Counters_t counters;
    AMW_Sequencer* sequencer = AMW_Sequencer::getInstance();
    counters.completedPackages = sequencer->completedPackages;
    counters.failedPackages = sequencer->failedPackages;
    counters.totalEmergencyLandings = sequencer->totalEmergencyLandings;
    counters.totalReturnToStarts = sequencer->totalReturnToStarts;
    counters.distance = distanceTravelled;
    return counters;
}

void AMW_Planner::resetLogging() {
    AMW_Sequencer::getInstance()->resetLogging();
    distanceTravelled = 0;
    lastPosition = AC_Facade::getFacade()->getRealPosition();
}
