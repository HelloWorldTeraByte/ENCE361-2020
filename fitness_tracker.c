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
#include "acc.h"
#include "i2c_driver.h"
#include "acc_reader.h"
#include "circBufT.h"
#include "buttons4.h"
#include "fitness_tracker.h"

static circBuf_t circbuf_x;
static circBuf_t circbuf_y;
static circBuf_t circbuf_z;

uint32_t run_btn;
uint32_t run_oled;
uint32_t run_acc;

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

/*********************************************************
 * initDisplay
 *********************************************************/
void
initDisplay (void)
{
    // Initialise the Orbit OLED display
    OLEDInitialise ();
}

//*****************************************************************************
// Function to display a changing message on the display.
// The display has 4 rows of 16 characters, with 0, 0 at top left.
//*****************************************************************************
void
displayUpdate (char *str1, char *str2, int16_t num, uint8_t charLine)
{
    char text_buffer[17];           //Display fits 16 characters wide.

    // "Undraw" the previous contents of the line to be updated.
    OLEDStringDraw ("                ", 0, charLine);
    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf(text_buffer, sizeof(text_buffer), "%s %s %3d", str1, str2, num);
    // Update line on display.
    OLEDStringDraw (text_buffer, 0, charLine);
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
    uint16_t i;
    int32_t sum_x, sum_y, sum_z;
    vector3_t ref_ori;
    vector3_t acc_mean;

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


    OLEDStringDraw ("Acc", 0, 0);

    while (1) {
        SysCtlDelay (SysCtlClockGet () / 150);

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

        if(run_acc > RUN_ACC_MAX) {
            run_acc = 0;
            acc_buff_write();
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
            displayUpdate ("Accl", "X", acc_mean.x - ref_ori.x , 1);
            displayUpdate ("Accl", "Y", acc_mean.y - ref_ori.y , 2);
            displayUpdate ("Accl", "Z", acc_mean.z - ref_ori.z , 3);
        }
    }
}
