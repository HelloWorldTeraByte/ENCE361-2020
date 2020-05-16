#ifndef FSM_H_
#define FSM_H_

#include "oled.h"
#include "acc_reader.h"

#define ACC_X_NOM 260.0
#define ACC_Y_NOM 262.0
#define ACC_Z_NOM 233.0

enum states
{
    steps, distance
};

void state_update(enum states *state, char oled_buffer[OLED_ROW_MAX][OLED_COL_MAX], vector3_t acc_mean);

#endif