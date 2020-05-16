#include <stdint.h>
#include "fsm.h"
#include "buttons4.h"
#include "oled.h"

void state_update(char oled_content[OLED_ROW_MAX][OLED_COL_MAX], uint32_t steps)
{
    static enum states state = STEPS;
    uint8_t up_btn_state, down_btn_state, left_btn_state, right_btn_state;

    up_btn_state = checkButton(UP);
    down_btn_state = checkButton(DOWN);
    left_btn_state = checkButton(LEFT);
    right_btn_state = checkButton(RIGHT);

    switch (state)
    {
        case STEPS:
            break;
        case DISTANCE:
            break;
        default:
            break;
    }
}
