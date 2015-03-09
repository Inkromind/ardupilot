/*
 * AMWTaskCommandTakeoff.h
 *
 *  Created on: 9-mrt.-2015
 *      Author: Arne
 */

#ifndef AMWTASKCOMMANDTAKEOFF_H_
#define AMWTASKCOMMANDTAKEOFF_H_

class AMW_Task_Command_Takeoff : public AMW_Task_Command {
public:
    virtual ~AMW_Task_Command_Takeoff();
    AMW_Task_Command_Takeoff(float altitude);

    void updateStatus(void);

    void runCommand(void);

    bool isComplete(void);

private:
    float altitude;
};

#endif /* AMWTASKCOMMANDTAKEOFF_H_ */
