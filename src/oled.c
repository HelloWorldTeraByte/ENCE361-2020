#include <stdint.h>
#include "utils/ustdlib.h"
#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "oled.h"

/* Keep track of the current content so we only need to update it if content changed*/
static char oled_content[OLED_ROW_MAX][OLED_COL_MAX];

/* Setup the display*/
void initDisplay (void)
{
    /* Initialise the Orbit OLED display */
    OLEDInitialise ();
}

/* Update the entire OLED display*/
void oled_update(char cont[OLED_ROW_MAX][OLED_COL_MAX])
{
    int i;
    for(i = 0; i < OLED_ROW_MAX; i++) {

        /* Only update it if the contenet has changed*/
        if(PARTIAL_OLED_UPDATE) {
            /* Is the buffer and the content on the display different*/
            if(ustrncmp(oled_content[i], cont[i], OLED_COL_MAX) != 0) {
                OLEDStringDraw("                ", 0, i);
                OLEDStringDraw(cont[i], 0, i);
                ustrncpy(oled_content[i], cont[i], OLED_COL_MAX);
            }
        }
        else {
            OLEDStringDraw("                ", 0, i);
            OLEDStringDraw(cont[i], 0, i);
         }
    }
}

/* Update a single line in the OLED display*/
void oled_update_line(char cont[OLED_COL_MAX], uint8_t i)
{
    /* Only update it if the contenet has changed*/
    if(PARTIAL_OLED_UPDATE) {
        /* Is the buffer and the content on the display different*/
        if(ustrncmp(oled_content[i], cont, OLED_COL_MAX) != 0) {
            OLEDStringDraw("                ", 0, i);
            OLEDStringDraw(cont, 0, i);
            ustrncpy(oled_content[i], cont, OLED_COL_MAX);
        }
    }
    else {
        OLEDStringDraw("                ", 0, i);
        OLEDStringDraw(cont, 0, i);
    }
}
