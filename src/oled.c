#include <stdint.h>
#include "utils/ustdlib.h"
#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "oled.h"

static char oled_content[OLED_ROW_MAX][OLED_COL_MAX];
/*********************************************************
 * initDisplay
 *********************************************************/
void initDisplay (void)
{
    // Initialise the Orbit OLED display
    OLEDInitialise ();
}

void oled_update(char cont[OLED_ROW_MAX][OLED_COL_MAX])
{
    int i;
    for(i = 0; i < OLED_ROW_MAX; i++) {

        if(PARTIAL_OLED_UPDATE) {
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

void oled_update_line(char cont[OLED_COL_MAX], uint8_t i)
{
    if(PARTIAL_OLED_UPDATE) {
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