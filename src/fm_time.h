#ifndef FM_TIME_H_ 
#define FM_TIME_H_ 

#define SYSTICK_RUN_MS 1
#define SYSTICK_RATE_HZ (1000/SYSTICK_RUN_MS)
#define TICK_RUN_MS SYSTICK_RUN_MS

#define RUN_1MS (1 / TICK_RUN_MS)
#define RUN_10MS (10 / TICK_RUN_MS)
#define RUN_100MS (100 / TICK_RUN_MS)
#define RUN_1000MS (1000 / TICK_RUN_MS)

void SysTickIntHandler(void);
void clock_init(void);
void fm_time_init(void);
uint32_t fm_get_tick(void);

#endif
