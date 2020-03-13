#ifndef ACC_READER_H_
#define ACC_READER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} vector3_t;

void initAccl (void);
vector3_t getAcclData (void);
vector3_t getAcclDataCmS2 (void);

#endif
