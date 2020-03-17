#ifndef FSM_H_
#define FSM_H_

#include "oled.h"

#define ACC_X_NOM 260.0
#define ACC_Y_NOM 262.0
#define ACC_Z_NOM 233.0

enum states
{
    acc_data, total_distance, set_goal
};

enum m1_states
{
    raw_acceleration, acceleration_g, acceleration_ms
};

void state_update(enum states *state, enum m1_states *m1_state,
                  char oled_content[OLED_ROW_MAX][OLED_COL_MAX], vector3_t acc_mean);

#endif
