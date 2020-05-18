#ifndef ACC_READER_H_
#define ACC_READER_H_

#include "circBufT.h"

#define ACC_BUF_SIZE 10

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} vector3_t;

void initAccl(void);
vector3_t getAcclData(void);
vector3_t getAcclDataCmS2(void);

int32_t acc_mean_calc(int32_t sum);
void acc_buff_write(circBuf_t *x_buff, circBuf_t *y_buff, circBuf_t *z_buff);
vector3_t acc_mean_get(circBuf_t *x_buff, circBuf_t *y_buff, circBuf_t *z_buff);
vector3_t acc_ref_get(circBuf_t *x_buff, circBuf_t *y_buff, circBuf_t *z_buff, uint8_t startup);

#endif
