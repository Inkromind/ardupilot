/*
 * AMW_Corridor_Manager.h
 *
 *  Created on: 11-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_CORRIDOR_MANAGER_H_
#define AMW_CORRIDOR_MANAGER_H_

#include <AMW_Modules.h>
#include <AMW_List.h>

class AMW_Corridor_Manager {
public:
    virtual ~AMW_Corridor_Manager() { }

    virtual void clearReservedCorridors(AMW_Module_Identifier* module);

    virtual bool hasReservedCorridors(AMW_Module_Identifier* module);

    virtual AMW_Corridor_Reservation_Identifier* startCorridorReservation(AMW_Module_Identifier* module, AMW_List<AMW_Corridor*>* corridors);

    static AMW_Corridor_Manager* getInstance(void);

private:
    static AMW_Corridor_Manager* module;
    AMW_Corridor_Manager();


};

#endif /* AMW_CORRIDOR_MANAGER_H_ */
