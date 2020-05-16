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

enum states state = steps;

char oled_buffer[OLED_ROW_MAX][OLED_COL_MAX];

uint32_t acc_ticks, bk_proc_tick, disp_ticks;

void acc_buff_write(void)
{
    vector3_t acc = getAcclData();

    writeCircBuf(&circbuf_x, acc.x);
    writeCircBuf(&circbuf_y, acc.y);
    writeCircBuf(&circbuf_z, acc.z);
}

int32_t mean_calc(int32_t sum)
{
    return ((2 * sum + ACC_BUF_SIZE) / 2 / ACC_BUF_SIZE);
}

void task_acc()
{
    acc_buff_write();
    acc_ticks++;
}
void task_bk_proc()
{
    uint16_t i;
    int32_t sum_x, sum_y, sum_z = 0;
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

    state_update(&state, oled_buffer, acc_mean);
    bk_proc_tick++;
}

void task_display()
{
    disp_ticks++;

    usnprintf(oled_buffer[0], sizeof(oled_buffer[0]), "acc: %d", acc_ticks);
    usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "bk: %d", bk_proc_tick);
    usnprintf(oled_buffer[2], sizeof(oled_buffer[2]), "dp: %d", disp_ticks);

    oled_update(oled_buffer);
}

void fm_init()
{
    //Disable the interupts to processor before setups
    IntMasterDisable();

    clock_init();
    fm_time_init();
    kernel_init(MAX_NUM_TASKS);
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

void fm_add_tasks()
{
    kernel_task_add(task_acc, ACC_RUN_MS);
    kernel_task_add(task_bk_proc, BK_PROC_RUN_MS);
    kernel_task_add(task_display, DISPLAY_RUN_MS);
}

void main(void)
{
    fm_init();

    //ref_ori = ref_ori_get(1);
    fm_add_tasks();
    kernel_run();
}