/*
 * AMW_Planner.cpp
 *
 *  Created on: 13-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Planner.h"
#include "AMW_Sequencer.h"
#include "AMW_Task_Planner.h"

void AMW_Planner::initPlanner() {
    AMW_Task_Planner::getInstance()->init();
    AMW_Sequencer::getInstance()->init();
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

}
