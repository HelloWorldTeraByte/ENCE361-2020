#ifndef FSM_H_
#define FSM_H_

#include "oled.h"

/* Nomial reading of the accerlerometer */
#define ACC_X_NOM 260.0
#define ACC_Y_NOM 262.0
#define ACC_Z_NOM 233.0

/* How much to increase and decrease the steps by in test mode*/
#define TEST_MODE_STEP_INC 100
#define TEST_MODE_STEP_DEC 500

/* Scaler to turn KMs to Miles*/
#define KM_TO_MILE 0.621371 

/* Main states the FM can be in */
enum states
{
    STEPS, DISTANCE
};

/* Sub sates in the distance state*/
enum distance_states
{
    DISTANCE_KM, DISTANCE_MILE
};

/* Called periodicaly to update the current state depending on the inputs*/
void state_update(char oled_buffer[OLED_ROW_MAX][OLED_COL_MAX], uint32_t *steps);

/* Check if we are in the testing mode*/
uint8_t check_testing_mode();

#endif
