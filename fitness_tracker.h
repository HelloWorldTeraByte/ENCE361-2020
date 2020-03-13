#ifndef FITNESS_TRACKER_H_
#define FITNESS_TRACKER_H_

/**********************************************************
 * Constants
 **********************************************************/
// Systick configuration
#define SYSTICK_RATE_HZ    100
#define ACC_BUF_SIZE 10


void initClock (void);
void initDisplay (void);
void SysTickIntHandler(void);

void displayUpdate (char *str1, char *str2, int16_t num, uint8_t charLine);

void acc_buff_write(void);
int32_t mean_calc(int32_t sum);
vector3_t ref_ori_get(void);
#endif
