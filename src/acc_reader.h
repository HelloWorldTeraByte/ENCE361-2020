#ifndef ACC_READER_H_
#define ACC_READER_H_

#define ACC_BUF_SIZE 10

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} vector3_t;

void initAccl (void);
vector3_t getAcclData (void);
vector3_t getAcclDataCmS2 (void);
vector3_t ref_ori_get(uint8_t startup);

#endif
