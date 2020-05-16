#include <stdint.h>
#include "fsm.h"
#include "oled.h"
#include "acc_reader.h"

void state_update(enum states *state, char oled_content[OLED_ROW_MAX][OLED_COL_MAX], vector3_t acc_mean)
{
    switch (*state)
    {
        case steps:
            break;
        default:
            break;
    }
}
