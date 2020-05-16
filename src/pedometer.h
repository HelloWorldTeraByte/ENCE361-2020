#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils/ustdlib.h"



#define norm_nom = 272
#define threshold = 13350//386


int32_t
norm(int32_t x_mean, int32_t y_mean, int32_t z_mean);

int
acc_check(int32_t acc_norm);

int
step_increment(int flag, int next_flag, int step_count);
