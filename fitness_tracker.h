#ifndef FITNESS_TRACKER_H_
#define FITNESS_TRACKER_H_

// Time schedulers
#define SYSTICK_RATE_HZ    100
#define RUN_BTN_RATE_HZ    100
#define RUN_ACC_RATE_HZ    100
#define RUN_OLED_RATE_HZ    2

#define ACC_BUF_SIZE 10

#define RUN_BTN_MAX SYSTICK_RATE_HZ/RUN_BTN_RATE_HZ
#define RUN_ACC_MAX SYSTICK_RATE_HZ/RUN_ACC_RATE_HZ
#define RUN_OLED_MAX SYSTICK_RATE_HZ/RUN_OLED_RATE_HZ


void initClock (void);
void initDisplay (void);
void SysTickIntHandler(void);

void displayUpdate (char *str1, char *str2, int16_t num, uint8_t charLine);

void acc_buff_write(void);
int32_t mean_calc(int32_t sum);
vector3_t ref_ori_get(uint8_t startup);

#endif
