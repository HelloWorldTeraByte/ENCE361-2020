#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "acc.h"
#include "i2c_driver.h"
#include "acc_reader.h"
#include "circBufT.h"
#include "buttons4.h"
#include "fsm.h"
#include "oled.h"


//char*
//float_string(float f)
//{
//    char c[3]; //size of the number
//    printf(c, "%f", f);
//    return c;
//}

void state_update(enum states *state, enum m1_states *m1_state,
                  char oled_content[OLED_ROW_MAX][OLED_COL_MAX], vector3_t acc_mean)
{
    uint8_t up_butState, down_butState, left_butState, right_butState;
    char text_buffer[17];
    char float_buff[5];

    vector3_t acc_g;

//    int8_t x_val = acc_mean.x;
//    int8_t y_val = acc_mean.y;
//    int8_t z_val = acc_mean.z;

    updateButtons();
    up_butState = checkButton(UP);
    down_butState = checkButton(DOWN);
    left_butState = checkButton(LEFT);
    right_butState = checkButton(RIGHT);

    switch (*state)
    {
    case acc_data:
        switch (*m1_state)
        {
        case raw_acceleration:
            //TODO: without ustrncpy?
            ustrncpy(oled_content[0], "Raw", 16);
            usnprintf(text_buffer, sizeof(text_buffer), "Acc X: %3d", acc_mean.x);
            ustrncpy(oled_content[1], text_buffer, 16);
            usnprintf(text_buffer, sizeof(text_buffer), "Acc Y: %3d", acc_mean.y);
            ustrncpy(oled_content[2], text_buffer, 16);
            usnprintf(text_buffer, sizeof(text_buffer), "Acc Z: %3d", acc_mean.z);
            ustrncpy(oled_content[3], text_buffer, 16);

            switch (up_butState)
            {
                case PUSHED:
                    break;
                case RELEASED:
                    *m1_state = acceleration_g;
                    break;
            }
            break;

        case acceleration_g:
            ustrncpy(oled_content[0], "In Gs", 16);

            usnprintf(text_buffer, sizeof(text_buffer), "Acc X: %s",
                      acc_mean.x/256);
            ustrncpy(oled_content[1], text_buffer, 16);

            usnprintf(text_buffer, sizeof(text_buffer), "Acc Y: %s",
                      acc_mean.x/256);
            ustrncpy(oled_content[2], text_buffer, 16);

            usnprintf(text_buffer, sizeof(text_buffer), "Acc Z: %s",
                      acc_mean.x/256);
            ustrncpy(oled_content[3], text_buffer, 16);

//            x_val = x_val / 256;
//            y_val = y_val / 256;
//            z_val = z_val / 256;

            switch (up_butState)
            {
                case PUSHED:
                    break;
                case RELEASED:
                    *m1_state = acceleration_ms;
                    break;
            }
            break;

        case acceleration_ms:
            ustrncpy(oled_content[0], "In ms-2s", 16);
            usnprintf(text_buffer, sizeof(text_buffer), "Acc X: %2d.%2d",
                      (((acc_mean.x*981)/256)/100), (((acc_mean.x*981)/256)%100));
            ustrncpy(oled_content[1], text_buffer, 16);
            usnprintf(text_buffer, sizeof(text_buffer), "Acc Y: %2d.%2d",
                      (((acc_mean.y*981)/256)/100), (((acc_mean.y*981)/256)%100));
            ustrncpy(oled_content[2], text_buffer, 16);
            usnprintf(text_buffer, sizeof(text_buffer), "Acc Z: %2d.%2d",
                      (((acc_mean.z*981)/256)/100), (((acc_mean.z*981)/256)%100));
            ustrncpy(oled_content[3], text_buffer, 16);
            //ustrncpy(oled_content[0], "accel in m/s/s", 16);

            //x_val / 26;
            //y_val / 26;
            //z_val / 26;

            switch (up_butState)
            {
                case PUSHED:
                    break;
                case RELEASED:
                    *m1_state = raw_acceleration;
                    break;
            }
            break;
        }

    case total_distance:
        //butState_2 = checkButton(RIGHT);
        //butState_3 = checkButton(LEFT);
        break;

    case set_goal:
        break;

    default:
        break;
    }
}
