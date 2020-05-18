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

enum distance_states
{
    DISTANCE_KM, DISTANCE_MILE
};

#define TEST_MODE_STEP_INC 100
#define TEST_MODE_STEP_DEC 500
#define KM_TO_MILE 0.621371 

void state_update(char oled_buffer[OLED_ROW_MAX][OLED_COL_MAX], uint32_t *steps);
uint8_t check_testing_mode();

#endif