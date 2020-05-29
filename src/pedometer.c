#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "pedometer.h"
#include "acc_reader.h"
#include "acc_reader.h"

static int32_t acc_norm;
/*Used to check if the acceleration is above the threshold and below it before*/
static uint8_t acc_flag = 0;

/* Check if a valid step is taken and increase steps by one*/
void steps_count_update(vector3_t acc_mean, uint32_t *steps_count)
{
    acc_norm = acc_norm_calc(acc_mean.x, acc_mean.y, acc_mean.z);
    /* Acceleration is above the threshold and previously below it*/
    if ((acc_norm > ACC_NORM_THRESHOLD) && acc_flag) {
        (*steps_count)++;
        acc_flag = 0;
    }
    if ((acc_norm < ACC_NORM_THRESHOLD) && !acc_flag) {
        acc_flag = 1;
    }
}

/* Get the norm of all axis acceleration*/
int32_t acc_norm_calc(int32_t x_mean, int32_t y_mean, int32_t z_mean)
{
    int32_t norm;
    norm = (sqrt((x_mean * x_mean) + (y_mean * y_mean) + (z_mean * z_mean)));
    return norm;
}
