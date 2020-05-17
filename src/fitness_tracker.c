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
#include "acc.h"
#include "i2c_driver.h"
#include "acc_reader.h"
#include "circBufT.h"
#include "buttons4.h"
#include "oled.h"
#include "fsm.h"
#include "fm_time.h"
#include "kernel.h"

static circBuf_t circbuf_x;
static circBuf_t circbuf_y;
static circBuf_t circbuf_z;

static char oled_buffer[OLED_ROW_MAX][OLED_COL_MAX];
static uint32_t steps;
static uint32_t io_btns_ticks, acc_ticks, bk_proc_tick, disp_ticks;

int32_t mean_calc(int32_t sum)
{
    return ((2 * sum + ACC_BUF_SIZE) / 2 / ACC_BUF_SIZE);
}

void acc_buff_write(void)
{
    vector3_t acc = getAcclData();

    writeCircBuf(&circbuf_x, acc.x);
    writeCircBuf(&circbuf_y, acc.y);
    writeCircBuf(&circbuf_z, acc.z);
}

void task_io_btns(void)
{
    io_btns_ticks++;
    updateButtons();
}

void task_acc(void)
{
    acc_buff_write();
    acc_ticks++;
}

//Background processing is done in a round robin 
void task_bk_proc(void)
{
    uint16_t i;
    int32_t sum_x = 0, sum_y = 0, sum_z = 0;
    //vector3_t ref_ori;
    vector3_t acc_mean;

    for (i = 0; i < ACC_BUF_SIZE; i++) {
        sum_x = sum_x + readCircBuf(&circbuf_x);
        sum_y = sum_y + readCircBuf(&circbuf_y);
        sum_z = sum_z + readCircBuf(&circbuf_z);
    }

    acc_mean.x = mean_calc(sum_x);
    acc_mean.y = mean_calc(sum_y);
    acc_mean.z = mean_calc(sum_z);

    steps++;
    state_update(oled_buffer, &steps);
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

    initCircBuf(&circbuf_x, ACC_BUF_SIZE);
    initCircBuf(&circbuf_y, ACC_BUF_SIZE);
    initCircBuf(&circbuf_z, ACC_BUF_SIZE);

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
