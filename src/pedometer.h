#ifndef PEDOMETER_H_
#define PEDOMETER_H_

#include <stdint.h>
#include "acc_reader.h"

#define GRAV_ACC 9.81
#define THRESH_GS 1.5
#define NORM_NOM 252
//#define ACC_NORM_THRESHOLD (GRAV_ACC * THRESH_GS * NORM_NOM)  //9.81 x 1.5 , or 9.8 x 1.5 = 14.7. The accelerometer sensitivity threshold. 257 x 1.5 = 385.5
#define ACC_NORM_THRESHOLD 386  //9.81 x 1.5 , or 9.8 x 1.5 = 14.7. The accelerometer sensitivity threshold. 257 x 1.5 = 385.5

void steps_count_update(vector3_t acc_mean, uint32_t *steps_count);
int32_t acc_norm_calc(int32_t x_mean, int32_t y_mean, int32_t z_mean);

#endif
