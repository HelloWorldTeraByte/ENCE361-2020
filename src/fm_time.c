#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"

#include "fm_time.h"

/* Used to keep track of the time; the var is incremented every tick of kernel frequency */
static uint32_t fm_tick = 0;

/* The ISR is called every 10ms, the fm_tick is used to measure the time */
void SysTickIntHandler(void)
{
    fm_tick++;      /* Updated every systick in this case 10ms*/
}

/* Called in startup to setup the main clock*/
void clock_init(void)
{
    /* Set the clock rate to 20 MHz */
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
}


/* Called in startup to setup the systick timer*/
void fm_time_init(void)
{
    /* Set up the period for the SysTick timer.  The SysTick timer period is */
    /* set as a function of the system clock. */
    SysTickPeriodSet(SysCtlClockGet() / SYSTICK_RATE_HZ);

    /* Register the interrupt handler */
    SysTickIntRegister(SysTickIntHandler);

    /* Enable interrupt and device */
    SysTickIntEnable();
    SysTickEnable();

}

/* Get the current tick, called externally to access the time*/
uint32_t fm_get_tick(void)
{
    return fm_tick;
}
