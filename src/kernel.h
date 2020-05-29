#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdint.h>

/* For each task a task object is created and stored in array */
typedef struct 
{
    void (*handle) (void);  /* Function handler for the task */
    uint16_t interval;      /* Used to set the frequency of the task */
    uint32_t ran_tick;      /* The last tick the tasks ran */
} task_t;

/* Called at startup to initialize the kernel*/
int8_t kernel_init(uint16_t max_tasks);

/* Add a task to the kernel, insert highest priority first*/
int8_t kernel_task_add(void (*func_ptr)(void), uint16_t interval);

/* Infinite loop of the kernel */
void kernel_run();

#endif
