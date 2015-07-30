/*
 * AC_Behaviour_Battery_Monitor.cpp
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#include "AC_Behaviour_Battery_Monitor.h"
#include <AC_ReactiveFacade.h>
#include <AC_CommunicationFacade.h>
#include "AC_SafetyControl.h"
#include <AMW_Corridors.h>
#include <AMW_Facade.h>
#include "../AP_Math/vector3.h"
#include "../AP_Math/vector2.h"

bool AC_Behaviour_Battery_Monitor::isActive(void) {
    if (activated)
        return true;
    if (AC_ReactiveFacade::getReactiveFacade()->getBattery()->capacity_remaining_pct() < AC_BATTERY_LIMIT) {
#ifdef AC_SAFETYCONTROL_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("Battery Low! Landing..."));
#endif
        activated = true;
        AMW_Facade::markBatteryEmpty();
    }
    return activated;
}

bool AC_Behaviour_Battery_Monitor::perform() {
    if (activated) {
        if (AC_ReactiveFacade::getReactiveFacade()->isLanded()) {
            if (landCorridor) {
                AMW_List<AMW_Corridor*> corridors;
                corridors.push_back(landCorridor);
                AMW_Facade::freeCorridors(&corridors);
                landCorridor = 0;
            }
            if (AC_ReactiveFacade::getReactiveFacade()->areMotorsArmed())
                return AC_ReactiveFacade::getReactiveFacade()->disarmMotors();
            else
                return true;
        }
        else {

            bool landing = AC_ReactiveFacade::getReactiveFacade()->land();
            if (!landCorridor && landing) {
                Vector3f location = AC_ReactiveFacade::getReactiveFacade()->getRealPosition();
                landCorridor = new AMW_Vertical_Corridor(Vector2f(location.x, location.y), location.z);
                AMW_List<AMW_Corridor*> corridors;
                corridors.push_back(landCorridor);
                AMW_Facade::markCorridorsReserved(AC_SafetyControl::getModuleIdentifier(), &corridors);
            }
            return landing;
        }
    }
    return false;
}
