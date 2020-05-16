#ifndef FSM_H_
#define FSM_H_

#include "oled.h"

#define ACC_X_NOM 260.0
#define ACC_Y_NOM 262.0
#define ACC_Z_NOM 233.0

enum states
{
    STEPS, DISTANCE
};

void state_update(char oled_content[OLED_ROW_MAX][OLED_COL_MAX], uint32_t steps);

#endif