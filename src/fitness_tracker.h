#ifndef FITNESS_TRACKER_H_
#define FITNESS_TRACKER_H_

#include "acc_reader.h"
#include "fm_time.h"

#define MAX_NUM_TASKS 5

// Time schedulers
#define ACC_RUN_MS RUN_10MS
#define IO_BTNS_RUN_MS RUN_10MS
#define BK_PROC_RUN_MS RUN_100MS
#define DISPLAY_RUN_MS RUN_100MS

#define STEPS_TO_M 0.9
#define ACC_NORM_THRESHOLD 386

enum dp_task_states
{
    LINE0, LINE1, LINE2, LINE3, LINE4
};

void task_io_btns(void);
void task_acc(void);
void task_bk_proc(void);
void task_display(void);

void fm_init(void);
void fm_add_tasks(void);

#endif
