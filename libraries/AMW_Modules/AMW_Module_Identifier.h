/*
 * AMW_Module_Identifier.h
 *
 *  Created on: 11-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_MODULE_IDENTIFIER_H_
#define AMW_MODULE_IDENTIFIER_H_

#include <stdint.h>

class AMW_Module_Identifier {

public:
    AMW_Module_Identifier(uint8_t priorityCorridors = 0) { corridorPriority = priorityCorridors; };
    virtual ~AMW_Module_Identifier() { }

    virtual uint8_t getCorridorPriority() const {
        return corridorPriority;
    }

private:
    uint8_t corridorPriority;
};




#endif /* AMW_MODULE_IDENTIFIER_H_ */
