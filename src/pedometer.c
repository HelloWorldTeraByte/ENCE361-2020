#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "pedometer.h"
#include "acc_reader.h"

int32_t acc_norm;

uint8_t acc_thresh_check(int32_t acc_norm)
{
    uint8_t flag;
    if (acc_norm < ACC_NORM_THRESHOLD)
        flag = 1;
    else
        flag = 0;
    return flag;
}

uint32_t steps_increment(uint8_t flag, uint8_t next_flag, uint32_t step_count)
{
    if (flag == 1 && next_flag == 0)
        step_count = step_count + 1;
    return step_count;
}

int32_t acc_norm_calc(int32_t x_mean, int32_t y_mean, int32_t z_mean)
{
    int32_t norm;
    //uint32_t norm_in_g;
    norm = (sqrt((x_mean * x_mean) + (y_mean * y_mean) + (z_mean * z_mean)));
    //norm_in_g = (int)(((norm/norm_nom)-((int)(norm/norm_nom)))*100));
    return norm;
}

int32_t acc_mean_calc(int32_t sum)
{
    return ((2 * sum + ACC_BUF_SIZE) / 2 / ACC_BUF_SIZE);
}