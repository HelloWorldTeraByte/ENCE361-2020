#ifndef SWITCHES_H_
#define SWITCHES_H_

#define NUM_SW 2

enum switches {SW1 = 0, SW2};
enum sw_state {SW_LOW = 0, SW_HIGH};

/* Default position of the switch*/
#define SW_ACTIVE_HIGH  1
#define SW_ACTIVE_LOW  0

/* SW1 location*/
#define SW1_PERIPH  SYSCTL_PERIPH_GPIOA
#define SW1_PORT_BASE  GPIO_PORTA_BASE
#define SW1_PIN  GPIO_PIN_7

/* SW2 location*/
#define SW2_PERIPH  SYSCTL_PERIPH_GPIOA
#define SW2_PORT_BASE  GPIO_PORTA_BASE
#define SW2_PIN  GPIO_PIN_6

/* Used for the debouncing count*/
#define SW_POLLS 3

/* Initialize the switches IO*/
void switches_init(void);

/* Poll the switches*/
void switches_update(void);

/* Get switch status from the last poll */
uint8_t switches_get(uint8_t sw);

#endif
