#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdint.h>

typedef struct 
{
    void (*handle) (void);  //Function handler for the task
    uint16_t interval;
    uint32_t ran_tick;
} task_t;

int8_t kernel_init(uint16_t max_tasks);
int8_t kernel_task_add(void (*func_ptr)(void), uint16_t interval);
void kernel_run();

#endif

