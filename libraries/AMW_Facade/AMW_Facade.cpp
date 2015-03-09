/*
 * AMW_Facade.cpp
 *
 *  Created on: 10-feb.-2015
 *      Author: Arne
 */

#include "AMW_Facade.h"
#include <AMW_Planner_Sequencer.h>
#include <AMW_Task_Planner.h>

void AMW_Facade::initPlanner() {
    AMW_Task_Planner::getInstance()->init();
    AMW_Sequencer::getInstance()->init();
}
