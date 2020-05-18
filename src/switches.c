#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "switches.h"

static uint8_t sw_state[NUM_SW];
static uint8_t sw_count[NUM_SW];
static uint8_t sw_normal[NUM_SW];

void switches_init(void)
{
    int i;

    SysCtlPeripheralEnable(SW1_PERIPH);
    GPIOPinTypeGPIOInput(SW1_PORT_BASE, SW1_PIN);
    GPIOPadConfigSet(SW1_PORT_BASE, SW1_PIN, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPD);
    sw_normal[SW1] = !SW_ACTIVE_HIGH;

    SysCtlPeripheralEnable(SW2_PERIPH);
    GPIOPinTypeGPIOInput(SW2_PORT_BASE, SW2_PIN);
    GPIOPadConfigSet(SW2_PORT_BASE, SW2_PIN, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPD);
    sw_normal[SW2] = !SW_ACTIVE_HIGH;

    for (i = 0; i < NUM_SW; i++)
    {
        sw_state[i] = sw_normal[i];
		sw_count[i] = 0;
    }
}

void switches_update(void)
{
	uint8_t sw_value[NUM_SW];
	int i;
	
	sw_value[SW1] = (GPIOPinRead (SW1_PORT_BASE, SW1_PIN) == SW1_PIN);
	sw_value[SW2] = (GPIOPinRead (SW2_PORT_BASE, SW2_PIN) == SW2_PIN);

	for (i = 0; i < NUM_SW; i++)
	{
        if (sw_value[i] != sw_state[i]) {
        	sw_count[i]++;
        	if (sw_count[i] >= SW_POLLS)
        	{
        		sw_state[i] = sw_value[i];
        		sw_count[i] = 0;
        	}
        }
        else
        	sw_count[i] = 0;
    }
}

uint8_t switches_get(uint8_t sw)
{
    return sw_state[sw];
}
