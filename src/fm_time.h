#ifndef FM_TIME_H_ 
#define FM_TIME_H_ 

#define SYSTICK_RUN_MS 10 /* Desired systick period in ms*/
#define SYSTICK_RATE_HZ (1000/SYSTICK_RUN_MS) /* Used because initilization is done in frequency*/
#define TICK_RUN_MS SYSTICK_RUN_MS      /* For future if we decide to not use systick as the main clock */

/* Tasks use this for the desired timing*/
#define RUN_10MS (10 / TICK_RUN_MS) 
#define RUN_100MS (100 / TICK_RUN_MS)
#define RUN_1000MS (1000 / TICK_RUN_MS)

/* The ISR is called every 10ms, the fm_tick is used to measure the time */
void SysTickIntHandler(void);

/* Called in startup to setup the main clock*/
void clock_init(void);

/* Called in startup to setup the systick timer*/
void fm_time_init(void);

/* Get the current tick, called externally to access the time*/
uint32_t fm_get_tick(void);

#endif
