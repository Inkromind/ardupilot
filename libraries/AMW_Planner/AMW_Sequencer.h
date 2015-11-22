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
     *
     * The first task in the Task Planner will be executed.
     *
     * If the Task Planner supplies a new task before before the previous one
     * has been completed, it will pause the old task and run the new task
     * as a temporary task. Tasks marked as temporary will be "attempted" to
     * run and can fail faster.
     * If a temporary task fails, the old task will be resumed. The temporary
     * task will not be attempted again until the old task completes (and at
     * that point is still the first task). If a new first task is given before
     * the old task completes, this new task will also be executed as a
     * temporary task.
     *
     * If the Task Planner "forces" the first task, it will be run as the main
     * task and remove the old task and temporary task if they exist.
     * If the Task Planner "forces" a 0 task, the current and old task
     * will be deleted
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
     *
     * If it was already executing the first task in the Task Planner
     * as either the current task or the temporary task, and the Task Planner
     * is not forcing the task, it will resume the task as the current task
     * or temporary task depending on how it was executing it beforehand.
     *
     * If it was already executing the first task in the Task Planner
     * and the Task Planner is forcing the task, it will be resumed
     * as the current plan and any temporary task will be removed.
     *
     * If the Task Planner returns a 0 task, the task that was executing
     * will be resumed if the 0 task is not forced. If the 0 task is forced,
     * the Sequencer will loiter and remove any current and/or temporary task.
     *
     * If it was not executing the first task in the Task Planner,
     * it will be started as either a current task or a temporary task
     * similar to starting a new task during normal operation.
     */
    void resumeMission(void);

    void resetLogging();

    uint16_t totalReturnToStarts;
    uint16_t totalEmergencyLandings;
    uint16_t completedPackages;
    uint16_t failedPackages;

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
