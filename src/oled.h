#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

#define OLED_ROW_MAX 4
#define OLED_COL_MAX 17

#define PARTIAL_OLED_UPDATE 0

void initDisplay(void);
void oled_update(char cont[OLED_ROW_MAX][OLED_COL_MAX]);
void oled_update_line(char cont[OLED_COL_MAX], uint8_t i);

#endif
