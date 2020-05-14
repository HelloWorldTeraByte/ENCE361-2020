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
#include "math.h"

void state_update(enum states *state, enum m1_states *m1_state,
                  char oled_content[OLED_ROW_MAX][OLED_COL_MAX], vector3_t acc_mean)
{
    uint8_t up_butState, down_butState, left_butState, right_butState;

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
            usnprintf(oled_content[0], sizeof(oled_content[1]), "Raw Data");

            usnprintf(oled_content[1], sizeof(oled_content[1]), "Acc X: %3d", acc_mean.x);
            usnprintf(oled_content[2], sizeof(oled_content[2]), "Acc Y: %3d", acc_mean.y);
            usnprintf(oled_content[3], sizeof(oled_content[3]), "Acc Z: %3d", acc_mean.z);

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
            usnprintf(oled_content[0], sizeof(oled_content[1]), "In G\'s");


           // printf("%2d.%2d", (int)(acc_mean/256.0), (int)(((acc_mean/256.0)-((int)(acc_mean/256.0)))*100));
            if(acc_mean.x >= 0)
              usnprintf(oled_content[1], sizeof(oled_content[1]), "Acc X: %2d.%2d", (int)(acc_mean.x/ACC_X_NOM), (int)(((acc_mean.x/ACC_X_NOM)-((int)(acc_mean.x/ACC_X_NOM)))*100));
          else
              usnprintf(oled_content[1], sizeof(oled_content[1]), "Acc X: -%2d.%2d", abs((int)(acc_mean.x/ACC_X_NOM)), abs((int)(((acc_mean.x/ACC_X_NOM)-((int)(acc_mean.x/ACC_X_NOM)))*100)));

          if(acc_mean.y >= 0)
              usnprintf(oled_content[2], sizeof(oled_content[2]), "Acc Y: %2d.%2d", (int)(acc_mean.y/ACC_Y_NOM), (int)(((acc_mean.y/ACC_Y_NOM)-((int)(acc_mean.y/ACC_Y_NOM)))*100));
          else
              usnprintf(oled_content[2], sizeof(oled_content[2]), "Acc Y: -%2d.%2d", abs((int)(acc_mean.y/ACC_Y_NOM)), abs((int)(((acc_mean.y/ACC_Y_NOM)-((int)(acc_mean.y/ACC_Y_NOM)))*100)));

          if(acc_mean.z >= 0)
              usnprintf(oled_content[3], sizeof(oled_content[3]), "Acc Z: %2d.%2d", (int)(acc_mean.z/ACC_Z_NOM), (int)(((acc_mean.z/ACC_Z_NOM)-((int)(acc_mean.z/ACC_Z_NOM)))*100));
          else
              usnprintf(oled_content[3], sizeof(oled_content[3]), "Acc Z: -%2d.%2d", abs((int)(acc_mean.z/ACC_Z_NOM)), abs((int)(((acc_mean.z/ACC_Z_NOM)-((int)(acc_mean.z/ACC_Z_NOM)))*100)));

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

            usnprintf(oled_content[0], sizeof(oled_content[1]), "In ms-2");


            if(acc_mean.x >= 0)
                usnprintf(oled_content[1], sizeof(oled_content[1]), "Acc X: %2d.%2d", (int)((9.81/ACC_X_NOM)*acc_mean.x), (int)((((9.81/ACC_X_NOM)*acc_mean.x)-((int)((9.81/ACC_X_NOM)*acc_mean.x)))*100));
            else
                usnprintf(oled_content[1], sizeof(oled_content[1]), "Acc X: -%2d.%2d", abs((int)((9.81/ACC_X_NOM)*acc_mean.x)), abs((int)((((9.81/ACC_X_NOM)*acc_mean.x)-((int)((9.81/ACC_X_NOM)*acc_mean.x)))*100)));

            if(acc_mean.y >= 0)
                usnprintf(oled_content[2], sizeof(oled_content[2]), "Acc Y: %2d.%2d", (int)((9.81/ACC_Y_NOM)*acc_mean.y), (int)((((9.81/ACC_Y_NOM)*acc_mean.y)-((int)((9.81/ACC_Y_NOM)*acc_mean.y)))*100));
            else
                usnprintf(oled_content[2], sizeof(oled_content[2]), "Acc Y: -%2d.%2d", abs((int)((9.81/ACC_Y_NOM)*acc_mean.y)), abs((int)((((9.81/ACC_Y_NOM)*acc_mean.y)-((int)((9.81/ACC_Y_NOM)*acc_mean.y)))*100)));

            if(acc_mean.z >= 0)
                usnprintf(oled_content[3], sizeof(oled_content[3]), "Acc Z: %2d.%2d", (int)((9.81/ACC_Z_NOM)*acc_mean.z), (int)((((9.81/ACC_Z_NOM)*acc_mean.z)-((int)((9.81/ACC_Z_NOM)*acc_mean.z)))*100));
            else
                usnprintf(oled_content[3], sizeof(oled_content[3]), "Acc Z: -%2d.%2d", abs((int)((9.81/ACC_Z_NOM)*acc_mean.z)), abs((int)((((9.81/ACC_Z_NOM)*acc_mean.z)-((int)((9.81/ACC_Z_NOM)*acc_mean.z)))*100)));

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
