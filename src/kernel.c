#include <stdlib.h>
#include "kernel.h"
#include "fitness_tracker.h"
#include "fm_time.h"

task_t *tasks;
uint16_t num_tasks;

int8_t kernel_init(uint16_t max_tasks)
{
    num_tasks = 0;
    tasks = malloc(max_tasks*sizeof(task_t));
    if(tasks == NULL)
        return -1;
    else
        return 0;
}

int8_t kernel_task_add(void (*func_ptr)(void), uint16_t interval)
{
    //There isnt enought space to add the task return an error
    if(num_tasks >= MAX_NUM_TASKS)
        return -1;
    tasks[num_tasks].handle = func_ptr;
    tasks[num_tasks].interval = interval;
    num_tasks++;

    return 0;
}

void kernel_run()
{
    uint32_t tick = 0;
    uint16_t task_index = 0;

    while(1) {

        tick = fm_get_tick();

        for(task_index = 0; task_index < num_tasks; task_index++) {

            if( (tick - tasks[task_index].ran_tick) >= tasks[task_index].interval) {
                (*tasks[task_index].handle)();
                tasks[task_index].ran_tick = tick;
            }

        }
    }

}
