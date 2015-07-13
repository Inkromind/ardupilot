/*
 * AMW_Corridor_Manager.cpp
 *
 *  Created on: 11-jul.-2015
 *      Author: Arne
 */

#include "AMW_Corridor_Manager.h"

AMW_Corridor_Manager* AMW_Corridor_Manager::module = 0;

AMW_Corridor_Manager::AMW_Corridor_Manager() {
    // TODO Auto-generated constructor stub

}

AMW_Corridor_Manager* AMW_Corridor_Manager::getInstance(void) {
    if (!AMW_Corridor_Manager::module) {
        AMW_Corridor_Manager::module = new AMW_Corridor_Manager();
    }
    return AMW_Corridor_Manager::module;
}

