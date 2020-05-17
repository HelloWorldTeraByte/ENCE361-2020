#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils/ustdlib.h"

#define NORM_NOM  252
#define THRESHOLD 386  //9.81 x 1.5 , or 9.8 x 1.5 = 14.7. The accelerometer sensitivity threshold. 257 x 1.5 = 385.5

int32_t norm(int32_t x_mean, int32_t y_mean, int32_t z_mean);

int acc_check(int32_t acc_norm);

int step_increment(int flag, int next_flag, int step_count);
