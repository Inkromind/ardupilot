/*
 * AMW_Sequencer.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_SEQUENCER_H_
#define AMW_SEQUENCER_H_

#include "AMW_Task.h"
#include "AMW_Commands_Plan.h"

/**
 * Runs short-term goals/commands to complete the tasks
 * received by the Task Planner.
 */
class AMW_Sequencer {
public:
    /**
     * Run the sequencer
     */
    virtual void run();

    /**
     * Initialize the sequencer
     */
    virtual void init();

    static AMW_Sequencer* getInstance(void);

    /**
     * Pause the mission
     */
    void pauseMission(void);

    /**
     * Pause/Resume the mission
     */
    void toggleMission(void);

    /**
     * Resume the mission
     */
    void resumeMission(void);

protected:
    AMW_Sequencer();

    bool sequencerInitialized;
    static AMW_Sequencer* sequencer;
    bool paused;

    AMW_Task* currentTask;
    AMW_Task* tempTask;
    AMW_Command* currentPlan;
    AMW_Command* tempPlan;
    bool executingCurrentTask;

    /**
     * Start a new task
     */
    void startNewTask(AMW_Task* newTask);

    /**
     * Try to start a new task. If it does not start right away
     * continue executing the previous task
     */
    void tryNewTask(AMW_Task* newTask);

    /**
     * Execute the current task
     */
    void executeCurrentTask(void);

    /**
     * Try to execute a new task
     */
    void executeTempTask(void);

    /**
     * Check which task should be executed
     */
    void checkExecutingTask(void);

    /**
     * Cancel the new task and continue executing the current task
     *
     * @param removeNewTask Set to true if the new task should be forgotten
     */
    void cancelTempTaskContinueCurrentTask(bool removeNewTask = false);

    virtual ~AMW_Sequencer();

};

#endif /* AMW_SEQUENCER_H_ */
