#include <stdint.h>
#include "utils/ustdlib.h"
#include "fsm.h"
#include "fitness_tracker.h"
#include "buttons4.h"
#include "switches.h"
#include "oled.h"

/* Keep track of the main state */
static enum states state = STEPS;
/* Used to keep track if we in km or miles*/
static enum distance_states dist_state = DISTANCE_KM;

/* Called periodically to update the current state depending on the inputs*/
void state_update(char oled_buffer[OLED_ROW_MAX][OLED_COL_MAX], uint32_t *steps)
{
    uint8_t up_btn_state, down_btn_state, left_btn_state, right_btn_state;
    uint8_t testing_mode;

    /* Get the current state of the button */
    /*this does not directly check the current btn state but the state since last poll*/
    up_btn_state = checkButton(UP);
    down_btn_state = checkButton(DOWN);
    left_btn_state = checkButton(LEFT);
    right_btn_state = checkButton(RIGHT);

    /* Are we in the testing mode?*/
    testing_mode = check_testing_mode();

    /* FSM implementation */
    switch (state)
    {
        /* If we are in the steps state*/
        case STEPS:
            /* If we keep down btn held and not in the testing mode need to reset the steps*/
            if (btn_check_held(DOWN) && !testing_mode) {
                *steps = 0;
            }
            /* Output Logic; Output the current steps*/
            usnprintf(oled_buffer[0], sizeof(oled_buffer[0]), "Steps");
            usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "%d", *steps);

            /* Next state logic, act according to the button input*/
            /* If we press the right btn need to go the distance state*/
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
            /* If we press the left btn need to go the distance state*/
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
                    /* If we press the up btn in testing mode increse the steps*/
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
                    /* If we press the down btn in testing mode decrease the steps*/
                    if (testing_mode) {
                        /* Steps Cant go negative */
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
        /* If we are in the steps state*/
        /* Distance is calculated with steps and not stored*/
        case DISTANCE:
            /* If we keep down btn held and not in the testing mode need to reset the steps/distance*/
            if(btn_check_held(DOWN) && !testing_mode) {
                *steps = 0;
            }

            /* Print to the OLED according if we in km or miles*/
            if (dist_state == DISTANCE_KM) {
                /* Split the interger and decimal part to print to the OLED*/
                uint32_t int_part = (int)((*steps)*STEPS_TO_M/1000);
                uint16_t dec_part = (int)(1000*((*steps)*STEPS_TO_M/1000 - (int)((*steps)*STEPS_TO_M/1000)));
                /* OLED does not accept floating point numbers so need to print in intergers*/
                usnprintf(oled_buffer[0], sizeof(oled_buffer[0]), "Distance kms");
                usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "%2d.%03d", int_part, dec_part);
                //usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "%2d.%03d", (int)((*steps)*0.9/1000), (int)(1000*((*steps)*0.9/1000 - (int)((*steps)*0.9/1000))));
            }
            else {
                uint32_t int_part = (int)((*steps)*STEPS_TO_M*KM_TO_MILE/1000);
                uint16_t dec_part = (int)(1000*((*steps)*STEPS_TO_M*KM_TO_MILE/1000 - (int)((*steps)*STEPS_TO_M*KM_TO_MILE/1000)));
                /* OLED does not accept floating point numbers so need to print in intergers*/
                usnprintf(oled_buffer[0], sizeof(oled_buffer[0]), "Distance miles");
                usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "%2d.%03d", int_part, dec_part);
                //usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "%f", 0.08);
            }
            /* If we press the right btn need to go the steps state*/
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
            /* If we press the left btn need to go the steps state*/
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
                    /* If we press the up btn in testing mode increase the steps*/
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
                    /* If we press the down btn in testing mode decrease the steps*/
                    if (testing_mode) {
                        /* Steps Cant go negative */
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

/* Check if we are in the testing mode*/
uint8_t check_testing_mode()
{
    return switches_get(SW1);
}
