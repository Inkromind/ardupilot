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

#define AMW_COMMAND_COMPOSITE_NAV_DESTINATION_RADIUS 200.0f
#ifndef TESTENV
    #define AMW_COMMAND_ONLY_LAND_AT_BASE
#endif
#define AMW_COMMAND_HOVER_ALTITUDE 500

/**
 * A command/goal representing a takeoff, navigation to and landing at
 * another location.
 *
 * This command will be completed right away if already landed at the
 * destination.
 *
 * This command will attempt reserve 2 vertical corridors and 1 horizontal
 * representing its flight path before takeoff.
 * If reservation fails, the command will fail.
 * If a corridor conflict is detected during its flight, it will in first
 * instance turn back and land at its starting location before attempting
 * to reserve a new set of corridors.
 * If a corridor conflict is detected while flying back, it will land right
 * away and attempt to reserve a new set of corridors.
 *
 * If this command should only use 1 attempt to start, the reservation
 * process will fail after going through all the flight levels,
 * otherwise the default number of rounds will be used.
 */
class AMW_Command_Composite_Nav: public AMW_Command_Composite {
public:
    /**
     * @param destination The location to fly to and land at (relative to origin)
     */
    AMW_Command_Composite_Nav(Vector2f newDestination, bool returnHome = false);
    virtual ~AMW_Command_Composite_Nav() {
        clearReservedCorridors();
    }

    enum CommandState { INIT, WAITING_FOR_CORRIDORS, FAILED, COMPLETED, NORMAL, RETURNTOSTART, LAND };

protected:
    virtual void startCommand(bool attempt);

    virtual void updateStatus(void);

    virtual void completedSubCommand(void);

    /**
     * Make the necessary corridors for normal execution/return to start
     */
    void getCorridors(void);

    /**
     * Free and clear the reserved corridors
     */
    void clearReservedCorridors(void);

    CommandState currentState;
    Vector3f destination;
    Vector2f startLocation;

    AMW_List<AMW_Corridor*> corridors;
private:

    /**
     * Set the subcommands used for its normal execution
     */
    void setNormalSubCommands(void);

    /**
     * Return to start
     */
    void returnToStart(void);

    /**
     * Land
     */
    void land(void);

    bool returningHome;
};

#endif /* AMW_COMMAND_COMPOSITE_NAV_ASSIGNED_ALTITUDE_H_ */
