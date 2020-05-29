#ifndef FITNESS_TRACKER_H_
#define FITNESS_TRACKER_H_

#include "acc_reader.h"
#include "fm_time.h"

/* Needed for the kernel initlization */
#define MAX_NUM_TASKS 5

/* Frequency to run each task */
#define ACC_RUN_MS RUN_10MS
#define IO_BTNS_RUN_MS RUN_10MS
#define BK_PROC_RUN_MS RUN_100MS
#define DISPLAY_RUN_MS RUN_100MS

/* The multplier to turn steps to meter */
#define STEPS_TO_M 0.9

/* States to keep track of the OLED update, for co operative multitasking*/
enum dp_task_states
{
    LINE0, LINE1, LINE2, LINE3, LINE4
};

/* Tasks to be run by the kernel*/
void task_io_btns(void);

/* Task to handle all the accelerometer related things; reading and writing to buffs*/
void task_acc(void);

/* Background processing is done in a round robin without quantum time */
void task_bk_proc(void);

/* Update the OLED display, using cooperative multitasking*/
void task_display(void);

/* Main initialization for the fitness tracker*/
void fm_init(void);

/* Add the tasks than needs to be run */
void fm_add_tasks(void);

#endif
