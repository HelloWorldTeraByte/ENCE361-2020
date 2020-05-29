#ifndef PEDOMETER_H_
#define PEDOMETER_H_

#include <stdint.h>
#include "acc_reader.h"

/* Constant of acceleration gravity used in calculation*/
#define GRAV_ACC 9.81
/* Threshold to triger and step count increase*/
#define THRESH_GS 1.5
/*Nomial norm of all the axis*/
#define NORM_NOM 252
/*Nomial Y accereraltion*/
#define NOMINAL_Y_ACC 256

/*Acceleration to count the step*/
#define ACC_NORM_THRESHOLD (NOMINAL_Y_ACC * THRESH_GS)  //9.81 x 1.5 , or 9.8 x 1.5 = 14.7. The accelerometer sensitivity threshold. 257 x 1.5 = 385.5

void steps_count_update(vector3_t acc_mean, uint32_t *steps_count);
/* Get the norm of all axis acceleration*/
int32_t acc_norm_calc(int32_t x_mean, int32_t y_mean, int32_t z_mean);

#endif
