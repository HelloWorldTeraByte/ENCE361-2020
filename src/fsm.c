#include <stdint.h>
#include "utils/ustdlib.h"
#include "fsm.h"
#include "fitness_tracker.h"
#include "buttons4.h"
#include "oled.h"

void state_update(char oled_buffer[OLED_ROW_MAX][OLED_COL_MAX], uint32_t *steps)
{
    //TODO: Use integer maths
    uint32_t distance_km = STEPS_TO_KM * (*steps);
    uint32_t distance_mile = KM_TO_MILES * distance_km;

    //Remain in memory out of the scope of the functions as states should be remembered
    static enum states state = STEPS;
    static enum distance_states dist_state = DISTANCE_KM;

    uint8_t up_btn_state, down_btn_state, left_btn_state, right_btn_state;

    up_btn_state = checkButton(UP);
    down_btn_state = checkButton(DOWN);
    left_btn_state = checkButton(LEFT);
    right_btn_state = checkButton(RIGHT);

    switch (state)
    {
        case STEPS:
            if (btn_check_held(DOWN)) {
                *steps = 0;
                distance_km = 0;
                distance_mile = 0;
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

            break;
        case DISTANCE:
            if(btn_check_held(DOWN)) {
                *steps = 0;
                distance_km = 0;
                distance_mile = 0;
            }

            if (dist_state == DISTANCE_KM) {
                usnprintf(oled_buffer[0], sizeof(oled_buffer[0]), "Distance kms");
                usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "%2d.%3d", (int)(distance_km/100), (int)(1000*(distance_km/100 - (int)(distance_km/100))));
            }
            else {
                usnprintf(oled_buffer[0], sizeof(oled_buffer[0]), "Distance miles");
                usnprintf(oled_buffer[1], sizeof(oled_buffer[1]), "%3d", distance_mile);
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
                    if (dist_state == DISTANCE_KM)
                        dist_state = DISTANCE_MILE;
                    else
                        dist_state = DISTANCE_KM;
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
