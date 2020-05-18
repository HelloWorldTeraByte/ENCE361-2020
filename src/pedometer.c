#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "pedometer.h"
#include "acc_reader.h"
#include "acc_reader.h"

static int32_t acc_norm;
static uint8_t steps_flag = 0;
static uint8_t steps_nxt_flag = 1;
static uint8_t acc_flag = 0;

void steps_count_update(vector3_t acc_mean, uint32_t *steps_count)
{
    acc_norm = acc_norm_calc(acc_mean.x, acc_mean.y, acc_mean.z);
    if ((acc_norm > ACC_NORM_THRESHOLD) && acc_flag) {
        (*steps_count)++;
        acc_flag = 0;
    }
    if ((acc_norm < ACC_NORM_THRESHOLD) && !acc_flag) {
        acc_flag = 1;
    }
}

int32_t acc_norm_calc(int32_t x_mean, int32_t y_mean, int32_t z_mean)
{
    int32_t norm;
    norm = (sqrt((x_mean * x_mean) + (y_mean * y_mean) + (z_mean * z_mean)));
    return norm;
}