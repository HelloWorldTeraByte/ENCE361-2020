#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "fitness_tracker.h"
#include "acc.h"
#include "i2c_driver.h"
#include "acc_reader.h"
#include "circBufT.h"
#include "buttons4.h"
#include "oled.h"
#include "fsm.h"

static circBuf_t circbuf_x;
static circBuf_t circbuf_y;
static circBuf_t circbuf_z;

uint32_t run_btn;
uint32_t run_oled;
uint32_t run_acc;
uint32_t run_fsm;

void
acc_buff_write(void)
{
    vector3_t acc = getAcclData();

    writeCircBuf (&circbuf_x, acc.x);
    writeCircBuf (&circbuf_y, acc.y);
    writeCircBuf (&circbuf_z, acc.z);
}

void
SysTickIntHandler(void)
{
    run_acc++;
    run_btn++;
    run_oled++;
    run_fsm++;
}

/***********************************************************
 * Initialisation functions: clock, SysTick, PWM
 ***********************************************************
 * Clock
 ***********************************************************/
void
initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    //
    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet(SysCtlClockGet() / SYSTICK_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister(SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable();
    SysTickEnable();
}

int32_t
mean_calc(int32_t sum)
{
    return ((2 * sum + ACC_BUF_SIZE) / 2 / ACC_BUF_SIZE);
}

vector3_t
ref_ori_get(uint8_t startup)
{
    uint16_t i;
    int32_t sum_x = 0;
    int32_t sum_y = 0;
    int32_t sum_z = 0;

    vector3_t ref;

    if(startup) {
        for (i = 0; i < ACC_BUF_SIZE; i++) {
            acc_buff_write();
            SysCtlDelay (SysCtlClockGet () / 100);
        }
    }

    for (i = 0; i < ACC_BUF_SIZE; i++) {
        sum_x = sum_x + readCircBuf (&circbuf_x);
        sum_y = sum_y + readCircBuf (&circbuf_y);
        sum_z = sum_z + readCircBuf (&circbuf_z);
    }

    ref.x = mean_calc(sum_x);
    ref.y = mean_calc(sum_y);
    ref.z = mean_calc(sum_z);

    return ref;
}

/********************************************************
 * main
 ********************************************************/
void
main (void)
  {
    enum states state = acc_data;
    enum m1_states m1_state = raw_acceleration;

    uint16_t i;
    int32_t sum_x, sum_y, sum_z;
    vector3_t ref_ori;
    vector3_t acc_mean;
    char oled_content[OLED_ROW_MAX][OLED_COL_MAX];

    ustrncpy(oled_content[0], "acceleration" , 16);
    ustrncpy(oled_content[1], "accel x", 16);
    ustrncpy(oled_content[2], "accel y", 16);
    ustrncpy(oled_content[3], "accel z", 16);

    initClock();
    initAccl();
    initDisplay();
    initButtons();

    initCircBuf(&circbuf_x, ACC_BUF_SIZE);
    initCircBuf(&circbuf_y, ACC_BUF_SIZE);
    initCircBuf(&circbuf_z, ACC_BUF_SIZE);

    ref_ori = ref_ori_get(1);

    // Enable interrupts to the processor.
    IntMasterEnable();

    while (1) {
        SysCtlDelay (SysCtlClockGet () / 150);

        if(run_acc > RUN_ACC_MAX) {
           run_acc = 0;
           acc_buff_write();
        }

        if(run_fsm > RUN_FSM_MAX) {
            state_update(&state, &m1_state, oled_content, acc_mean);
        }


        if(run_btn > RUN_BTN_MAX) {
            run_btn = 0;
            uint8_t butState;
            updateButtons();
            butState = checkButton(DOWN);
            switch(butState) {
            case PUSHED:
               // b_down_bttn = 1;
                break;
            case RELEASED:
                ref_ori = ref_ori_get(0);
                break;
            }
        }

        sum_x = 0;
        sum_y = 0;
        sum_z = 0;

        for(i = 0; i < ACC_BUF_SIZE; i++) {
            sum_x = sum_x + readCircBuf (&circbuf_x);
            sum_y = sum_y + readCircBuf (&circbuf_y);
            sum_z = sum_z + readCircBuf (&circbuf_z);
        }

        acc_mean.x = mean_calc(sum_x);
        acc_mean.y = mean_calc(sum_y);
        acc_mean.z = mean_calc(sum_z);

        if(run_oled > RUN_OLED_MAX) {
            run_oled = 0;

            oled_update(oled_content);
//            displayUpdate ("Accl", "X", acc _mean.x - ref_ori.x , 1);
//            displayUpdate ("Accl", "Y", acc_mean.y - ref_ori.y , 2);
//            displayUpdate ("Accl", "Z", acc_mean.z - ref_ori.z , 3);
        }
    }
}
