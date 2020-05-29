#include <stdlib.h>
#include "kernel.h"
#include "fitness_tracker.h"
#include "fm_time.h"

static task_t *tasks; /* Handler for array of tasks */
static uint16_t num_tasks; /* Keep track of the number of tasks */

/* Called at startup to initialize the kernel*/
int8_t kernel_init(uint16_t max_tasks)
{
    num_tasks = 0;
    /* Allocate memory to store all the task objects*/
    tasks = malloc(max_tasks*sizeof(task_t));     /* Okay to use malloc as we only malloc in initialization and never realloc*/
    if(tasks == NULL)
        return -1;
    else
        return 0;
}

/* Add a task to the kernel, insert highest priority first*/
int8_t kernel_task_add(void (*func_ptr)(void), uint16_t interval)
{
    /* If there isn't enough space to add the task return an error */
    if(num_tasks >= MAX_NUM_TASKS)
        return -1;
    /* Store the task*/
    tasks[num_tasks].handle = func_ptr;
    tasks[num_tasks].interval = interval;
    num_tasks++;

    return 0;
}

/* Infinite loop of the kernel */
void kernel_run()
{
    uint32_t tick = 0;
    uint16_t task_index = 0;

    while(1) {

        /* Get the current time*/
        tick = fm_get_tick();

        /* Go throught each task in the inserted order*/
        for(task_index = 0; task_index < num_tasks; task_index++) {

            /* Is it time to run the task according to the desired frequency*/
            if( (tick - tasks[task_index].ran_tick) >= tasks[task_index].interval) {
                (*tasks[task_index].handle)();  /* Run the task through the function pointer*/
                tasks[task_index].ran_tick = tick; /* Make sure to store the ran time so we can run it again next time at desired freq.*/
            }

        }
    }

}
