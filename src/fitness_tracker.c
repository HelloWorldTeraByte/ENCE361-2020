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

static circBuf_t circbuf_x;
static circBuf_t circbuf_y;
static circBuf_t circbuf_z;

static char oled_buffer[OLED_ROW_MAX][OLED_COL_MAX];
static uint32_t io_btns_ticks, acc_ticks, bk_proc_tick, disp_ticks;

static uint32_t steps_count = 0;

void task_io_btns(void)
{
    io_btns_ticks++;
    updateButtons();
    switches_update();
}

void task_acc(void)
{
    acc_buff_write(&circbuf_x, &circbuf_y, &circbuf_z);
    acc_ticks++;
}

//Background processing is done in a round robin 
void task_bk_proc(void)
{
   //vector3_t ref_ori;
   vector3_t acc_mean;
   acc_mean = acc_mean_get(&circbuf_x, &circbuf_y, &circbuf_z);
   steps_count_update(acc_mean, &steps_count);

   state_update(oled_buffer, &steps_count);
   bk_proc_tick++;
}

void task_display(void)
{
    disp_ticks++;

    //Display update is broken into smaller chunks
    static enum dp_task_states dp_tsk_state = LINE0;
    switch (dp_tsk_state)
    {
        case LINE0:
            oled_update_line(oled_buffer[0], 0);
            dp_tsk_state = LINE1;
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

void fm_init(void)
{
    //Disable the interupts to processor before setups
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

    //TODO: DO we need to?
    ustrncpy(oled_buffer[0], " ", 16);
    ustrncpy(oled_buffer[1], " ", 16);
    ustrncpy(oled_buffer[2], " ", 16);
    ustrncpy(oled_buffer[3], " ", 16);

    // Enable interrupts to the processor.
    IntMasterEnable();
}

void fm_add_tasks(void)
{
    kernel_task_add(task_io_btns, IO_BTNS_RUN_MS);
    kernel_task_add(task_acc, ACC_RUN_MS);
    kernel_task_add(task_bk_proc, BK_PROC_RUN_MS);
    kernel_task_add(task_display, DISPLAY_RUN_MS);
}

void main(void)
{
    fm_init();

    kernel_init(MAX_NUM_TASKS);
    fm_add_tasks();
    kernel_run();
}