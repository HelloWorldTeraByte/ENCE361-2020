#ifndef ACC_READER_H_
#define ACC_READER_H_

#include "circBufT.h"

/* Circular buffer size to store the accerlerometer readings*/
#define ACC_BUF_SIZE 10

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} vector3_t;

/* Initialize the accelerometer*/
void initAccl(void);

/* Return the accelerometer data*/
vector3_t getAcclData(void);

/* Return the accelerometer data in cm*s-2 */
vector3_t getAcclDataCmS2(void);

/* Calculate the mean stored in the circular buffers*/
int32_t acc_mean_calc(int32_t sum);

/* Write the the circular buffers*/
void acc_buff_write(circBuf_t *x_buff, circBuf_t *y_buff, circBuf_t *z_buff);

/* Calculate the mean stored in the circular buffers*/
vector3_t acc_mean_get(circBuf_t *x_buff, circBuf_t *y_buff, circBuf_t *z_buff);

/* Store the current reading and use it as reference; i.e. the position is used as reference*/
vector3_t acc_ref_get(circBuf_t *x_buff, circBuf_t *y_buff, circBuf_t *z_buff, uint8_t startup);

#endif
