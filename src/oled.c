#include <stdint.h>
#include "utils/ustdlib.h"
#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "oled.h"

/*********************************************************
 * initDisplay
 *********************************************************/
void initDisplay (void)
{
    // Initialise the Orbit OLED display
    OLEDInitialise ();
}

//*****************************************************************************
// Function to display a changing message on the display.
// The display has 4 rows of 16 characters, with 0, 0 at top left.
//*****************************************************************************
void displayUpdate (char *str1, char *str2, int16_t num, uint8_t charLine)
{
    char text_buffer[17];           //Display fits 16 characters wide.

    // "Undraw" the previous contents of the line to be updated.
    OLEDStringDraw ("                ", 0, charLine);
    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf(text_buffer, sizeof(text_buffer), "%s %s %3d", str1, str2, num);
    // Update line on display.
    OLEDStringDraw (text_buffer, 0, charLine);
}

//TODO: only update if there is change
void oled_update(char cont[OLED_ROW_MAX][OLED_COL_MAX])
{
    int i;
    for(i = 0; i < OLED_ROW_MAX; i++) {
        OLEDStringDraw ("                ", 0, i);
        OLEDStringDraw (cont[i], 0, i);
    }
}

void oled_update_line(char cont[OLED_COL_MAX], uint8_t i)
{
    OLEDStringDraw ("                ", 0, i);
    OLEDStringDraw (cont, 0, i);
}

