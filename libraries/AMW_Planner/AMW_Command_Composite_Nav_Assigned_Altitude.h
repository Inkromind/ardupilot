/*
 * AMW_Command_Composite_Nav_Assigned_Altitude.h
 *
 *  Created on: 21-mei-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_COMPOSITE_NAV_ASSIGNED_ALTITUDE_H_
#define AMW_COMMAND_COMPOSITE_NAV_ASSIGNED_ALTITUDE_H_

#include "AMW_Command_Composite.h"
#include "../AP_Math/vector2.h"
#include "../AP_Math/vector3.h"
#include <AMW_List.h>
#include <AMW_Corridors.h>

class AMW_Command_Composite_Nav_Assigned_Altitude: public AMW_Command_Composite {
public:
    AMW_Command_Composite_Nav_Assigned_Altitude(Vector2f destination);
    virtual ~AMW_Command_Composite_Nav_Assigned_Altitude() {
        clearReservedCorridors();
    }

protected:
    virtual void startCommand(bool attempt);

    virtual void updateStatus(void);

    virtual void completedSubCommand(void);

private:

    enum CommandState { INIT, WAITING_FOR_CORRIDORS, FAILED, COMPLETED, NORMAL, RETURNTOSTART, LAND };

    CommandState currentState;
    Vector3f destination;
    Vector2f startLocation;

    AMW_List<AMW_Corridor*> corridors;

    void resetSubCommands(void);

    void setNormalSubCommands(void);
    void getCorridors(void);

    void clearReservedCorridors(void);

    void returnToStart(void);

    void land(void);
};

#endif /* AMW_COMMAND_COMPOSITE_NAV_ASSIGNED_ALTITUDE_H_ */
