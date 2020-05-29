#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "utils/ustdlib.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"

#include "OrbitOLED/OrbitOLEDInterface.h"

#include "fitness_tracker.h"
#include "fm_time.h"
#include "kernel.h"
#include "i2c_driver.h"
#include "acc.h"
#include "acc_reader.h"
#include "circBufT.h"
#include "buttons4.h"
#include "switches.h"
#include "fsm.h"
#include "oled.h"
#include "pedometer.h"

/* The accelerometer data is stored in a circular buffer for each axis */
static circBuf_t circbuf_x;
static circBuf_t circbuf_y;
static circBuf_t circbuf_z;

/* The content to write to the OLED display */
/* The wanted data is writen to the buffer and another task updates the display */
static char oled_buffer[OLED_ROW_MAX][OLED_COL_MAX];
static uint32_t io_btns_ticks, acc_ticks, bk_proc_tick, disp_ticks;

/* Main step counts, used to calculate the distance in km and miles */
static uint32_t steps_count = 0;

/* Task to handle the IO; buttons and switches*/
void task_io_btns(void)
{
    io_btns_ticks++;
    /* Poll the button and get the status of the button */
    updateButtons();
    /* Poll the switches and get the status */
    switches_update();
}

/* Task to handle all the accelerometer related things; reading and writing to buffs*/
void task_acc(void)
{
    /* Read the accelerometer and write to the buffs */
    acc_buff_write(&circbuf_x, &circbuf_y, &circbuf_z);
    acc_ticks++;
}

/* Background processing is done in a round robin without quantum time */
void task_bk_proc(void)
{
   vector3_t acc_mean;
   acc_mean = acc_mean_get(&circbuf_x, &circbuf_y, &circbuf_z);
   /* Detect if a valid step is taken and update the steps_count variable */
   steps_count_update(acc_mean, &steps_count);

   /* Handles state switiching and state outputs for the FSM */
   state_update(oled_buffer, &steps_count);
   bk_proc_tick++;
}

/* Update the OLED display, using cooperative multitasking*/
void task_display(void)
{
    disp_ticks++;

    /* Updating OLED takes a long time so broken in to small chunks */
    /* In each call only one line is updated */
    static enum dp_task_states dp_tsk_state = LINE0;
    switch (dp_tsk_state)
    {
        /* Begin here*/
        case LINE0:
            oled_update_line(oled_buffer[0], 0);
            dp_tsk_state = LINE1;   /* Next call should update line 2 */
            break;
        case LINE1:
            oled_update_line(oled_buffer[1], 1);
            dp_tsk_state = LINE2;
            break;
        case LINE2:
            oled_update_line(oled_buffer[2], 2);
            dp_tsk_state = LINE3;
            break;
        case LINE3:
            oled_update_line(oled_buffer[3], 3);
            dp_tsk_state = LINE0;
        default:
            break;
    }
}

/* Main initialization for the fitness tracker*/
void fm_init(void)
{
    /* Disable the interrupts while setting up */
    IntMasterDisable();

    clock_init();
    fm_time_init();

    initAccl();
    initDisplay();

    initButtons();
    switches_init();

    initCircBuf(&circbuf_x, ACC_BUF_SIZE);
    initCircBuf(&circbuf_y, ACC_BUF_SIZE);
    initCircBuf(&circbuf_z, ACC_BUF_SIZE);

    /* Reenable them */
    IntMasterEnable();
}

/* Add the tasks than needs to be run */
void fm_add_tasks(void)
{
    kernel_task_add(task_io_btns, IO_BTNS_RUN_MS);
    kernel_task_add(task_acc, ACC_RUN_MS);
    kernel_task_add(task_bk_proc, BK_PROC_RUN_MS);
    kernel_task_add(task_display, DISPLAY_RUN_MS);
}

void main(void)
{
    fm_init();          /* Initialize the main components : clocks, accelerometer and IO */
    kernel_init(MAX_NUM_TASKS);     /* Initialize the kernel, max number of task is used for memory allocation */
    fm_add_tasks();     /* Need to add the task before running the kernel */

    /* Run the kernel indefinitely */
    kernel_run();
}
