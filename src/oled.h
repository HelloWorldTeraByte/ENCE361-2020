#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

/* Character dimensions of the OLED*/
#define OLED_ROW_MAX 4
#define OLED_COL_MAX 17

/* Only update the OLED if the content has changed*/
#define PARTIAL_OLED_UPDATE 0

/* Setup the display*/
void initDisplay(void);

/* Update the entire OLED display*/
void oled_update(char cont[OLED_ROW_MAX][OLED_COL_MAX]);

/* Update a single line in the OLED display*/
void oled_update_line(char cont[OLED_COL_MAX], uint8_t i);

#endif
