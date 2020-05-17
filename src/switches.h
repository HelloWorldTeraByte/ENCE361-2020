#ifndef SWITCHES_H_
#define SWITCHES_H_

#define NUM_SW 2

enum switches {SW1 = 0, SW2};
enum sw_state {SW_LOW = 0, SW_HIGH};

#define SW_ACTIVE_HIGH  1
#define SW_ACTIVE_LOW  0

#define SW1_PERIPH  SYSCTL_PERIPH_GPIOA
#define SW1_PORT_BASE  GPIO_PORTA_BASE
#define SW1_PIN  GPIO_PIN_7

#define SW2_PERIPH  SYSCTL_PERIPH_GPIOA
#define SW2_PORT_BASE  GPIO_PORTA_BASE
#define SW2_PIN  GPIO_PIN_6

#define SW_POLLS 3

void switches_init(void);
void switches_update(void);

uint8_t switches_get(uint8_t sw);

#endif