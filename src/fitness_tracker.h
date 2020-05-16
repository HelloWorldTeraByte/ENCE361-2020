#ifndef FITNESS_TRACKER_H_
#define FITNESS_TRACKER_H_

#include "acc_reader.h"
#include "fm_time.h"

#define MAX_NUM_TASKS 10

// Time schedulers
#define RUN_BTN_RATE_HZ    100
#define RUN_ACC_RATE_HZ    100
#define RUN_FSM_RATE_HZ    50
#define RUN_OLED_RATE_HZ    5

#define ACC_RUN_MS RUN_1MS
#define BK_PROC_RUN_MS RUN_10MS
#define DISPLAY_RUN_MS RUN_100MS

void initClock (void);
void initDisplay (void);
void SysTickIntHandler(void);

void displayUpdate (char *str1, char *str2, int16_t num, uint8_t charLine);

void acc_buff_write(void);
int32_t mean_calc(int32_t sum);
vector3_t ref_ori_get(uint8_t startup);

#endif
