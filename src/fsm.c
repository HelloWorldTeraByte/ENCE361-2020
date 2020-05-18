#include <stdint.h>
#include "utils/ustdlib.h"
#include "fsm.h"
#include "fitness_tracker.h"
#include "buttons4.h"
#include "switches.h"
#include "oled.h"

static enum states state = STEPS;
static enum distance_states dist_state = DISTANCE_KM;

void state_update(char oled_buffer[OLED_ROW_MAX][OLED_COL_MAX], uint32_t *steps)
{
    uint8_t up_btn_state, down_btn_state, left_btn_state, right_btn_state;
    uint8_t testing_mode;

    up_btn_state = checkButton(UP);
    down_btn_state = checkButton(DOWN);
    left_btn_state = checkButton(LEFT);
    right_btn_state = checkButton(RIGHT);
    testing_mode = check_testing_mode();

    switch (state)
    {
        case STEPS:
            if (btn_check_held(DOWN) && !testing_mode) {
                *steps = 0;
            }
            //Output Logic
            usnprintf(oled_buffer[0], sizeof(oled_buffer[0]), "Steps");
            usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "%d", *steps);

            //Next state logic
            switch (right_btn_state)
            {
                case PUSHED:
                    state = DISTANCE;
                    break;
                case RELEASED:
                    break;
                case NO_CHANGE:
                    break;
                default:
                    break;
            }
            switch (left_btn_state)
            {
                case PUSHED:
                    state = DISTANCE;
                    break;
                case RELEASED:
                    break;
                case NO_CHANGE:
                    break;
                default:
                    break;
            }
            switch (up_btn_state)
            {
                case PUSHED:
                    if (testing_mode) {
                        *steps += TEST_MODE_STEP_INC;
                    }
                    break;
                case RELEASED:
                    break;
                case NO_CHANGE:
                    break;
                default:
                    break;
            }
            switch (down_btn_state)
            {
                case PUSHED:
                    if (testing_mode) {
                        //Steps Cant go negative
                        if(*steps >= TEST_MODE_STEP_DEC)
                            *steps -= TEST_MODE_STEP_DEC;
                        else
                            *steps = 0;
                    }

                    break;
                case RELEASED:
                    break;
                case NO_CHANGE:
                    break;
                default:
                    break;
            }
            break;
        case DISTANCE:
            if(btn_check_held(DOWN) && !testing_mode) {
                *steps = 0;
            }

            if (dist_state == DISTANCE_KM) {
                uint32_t int_part = (int)((*steps)*STEPS_TO_M/1000);
                uint16_t dec_part = (int)(1000*((*steps)*STEPS_TO_M/1000 - (int)((*steps)*STEPS_TO_M/1000)));
                usnprintf(oled_buffer[0], sizeof(oled_buffer[0]), "Distance kms");
                usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "%2d.%03d", int_part, dec_part);
                //usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "%2d.%03d", (int)((*steps)*0.9/1000), (int)(1000*((*steps)*0.9/1000 - (int)((*steps)*0.9/1000))));
            }
            else {
                uint32_t int_part = (int)((*steps)*STEPS_TO_M*KM_TO_MILE/1000);
                uint16_t dec_part = (int)(1000*((*steps)*STEPS_TO_M*KM_TO_MILE/1000 - (int)((*steps)*STEPS_TO_M*KM_TO_MILE/1000)));
                usnprintf(oled_buffer[0], sizeof(oled_buffer[0]), "Distance miles");
                usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "%2d.%03d", int_part, dec_part);
                //usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "%f", 0.08);
            }
            switch (right_btn_state)
            {
                case PUSHED:
                    state = STEPS;
                    break;
                case RELEASED:
                    break;
                case NO_CHANGE:
                    break;
                default:
                    break;
            }
            switch (left_btn_state)
            {
                case PUSHED:
                    state = STEPS;
                    break;
                case RELEASED:
                    break;
                case NO_CHANGE:
                    break;
                default:
                    break;
            }
            switch (up_btn_state)
            {
                case PUSHED:
                    if (testing_mode) {
                        *steps += TEST_MODE_STEP_INC;
                    }
                    else
                    {
                        if (dist_state == DISTANCE_KM)
                            dist_state = DISTANCE_MILE;
                        else
                            dist_state = DISTANCE_KM;
                    }
                    break;
                case RELEASED:
                    break;
                case NO_CHANGE:
                    break;
                default:
                    break;
            }
            switch (down_btn_state)
            {
                case PUSHED:
                    if (testing_mode) {
                        //Steps Cant go negative
                        if (*steps >= TEST_MODE_STEP_DEC)
                            *steps -= TEST_MODE_STEP_DEC;
                        else
                            *steps = 0;
                     }
                    break;
                case RELEASED:
                    break;
                case NO_CHANGE:
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

uint8_t check_testing_mode()
{
    return switches_get(SW1);
}
