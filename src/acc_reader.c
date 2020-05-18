/**********************************************************
 *
 * accelerometer.c
 *
 * Example code which reads acceleration in
 * three dimensions and displays the reulting data on
 * the Orbit OLED display.
 *
 *    C. P. Moore
 *    11 Feb 2020
 *
 **********************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "acc.h"
#include "i2c_driver.h"
#include "acc_reader.h"
#include "circBufT.h"
#include "pedometer.h"

/*********************************************************
 * initAccl
 *********************************************************/
void initAccl (void)
{
    char    toAccl[] = {0, 0};  // parameter, value

    /*
     * Enable I2C Peripheral
     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    /*
     * Set I2C GPIO pins
     */
    GPIOPinTypeI2C(I2CSDAPort, I2CSDA_PIN);
    GPIOPinTypeI2CSCL(I2CSCLPort, I2CSCL_PIN);
    GPIOPinConfigure(I2CSCL);
    GPIOPinConfigure(I2CSDA);

    /*
     * Setup I2C
     */
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);

    GPIOPinTypeGPIOInput(ACCL_INT2Port, ACCL_INT2);

    //Initialize ADXL345 Acceleromter

    // set +-2g, 10 bit resolution, active low interrupts
    toAccl[0] = ACCL_DATA_FORMAT;
    toAccl[1] = (ACCL_RANGE_2G | ACCL_FULL_RES);
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_PWR_CTL;
    toAccl[1] = ACCL_MEASURE;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);


    toAccl[0] = ACCL_BW_RATE;
    toAccl[1] = ACCL_RATE_100HZ;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_INT;
    toAccl[1] = 0x00;       // Disable interrupts from accelerometer.
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_X;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_Y;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_Z;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);
}

/********************************************************
 * Function to read accelerometer
 ********************************************************/
vector3_t getAcclData (void)
{
    char    fromAccl[] = {0, 0, 0, 0, 0, 0, 0}; // starting address, placeholders for data to be read.
    vector3_t acceleration;
    uint8_t bytesToRead = 6;

    fromAccl[0] = ACCL_DATA_X0;
    I2CGenTransmit(fromAccl, bytesToRead, READ, ACCL_ADDR);

    acceleration.x = (fromAccl[2] << 8) | fromAccl[1]; // Return 16-bit acceleration readings.
    acceleration.y = (fromAccl[4] << 8) | fromAccl[3];
    acceleration.z = (fromAccl[6] << 8) | fromAccl[5];

    return acceleration;
}

vector3_t getAcclDataCmS2 (void)
{
    vector3_t acceleration_raw;
    vector3_t acceleration_ms2;

    acceleration_raw = getAcclData();

    acceleration_ms2.x = (981*acceleration_raw.x)/256;
    acceleration_ms2.y = (981*acceleration_raw.y)/256;
    acceleration_ms2.z = (981*acceleration_raw.z)/256;

    return acceleration_ms2;
}

int32_t acc_mean_calc(int32_t sum)
{
    return ((2 * sum + ACC_BUF_SIZE) / 2 / ACC_BUF_SIZE);
}

void acc_buff_write(circBuf_t *x_buff, circBuf_t *y_buff, circBuf_t *z_buff)
{
    vector3_t acc = getAcclData();

    writeCircBuf(x_buff, acc.x);
    writeCircBuf(y_buff, acc.y);
    writeCircBuf(z_buff, acc.z);
}

vector3_t acc_mean_get(circBuf_t *x_buff, circBuf_t *y_buff, circBuf_t *z_buff)
{
    vector3_t acc_mean;
    uint16_t i;
    int32_t sum_x = 0, sum_y = 0, sum_z = 0;
 
    for (i = 0; i < ACC_BUF_SIZE; i++) {
        sum_x = sum_x + readCircBuf(x_buff);
        sum_y = sum_y + readCircBuf(y_buff);
        sum_z = sum_z + readCircBuf(z_buff);
    }

    acc_mean.x = acc_mean_calc(sum_x);
    acc_mean.y = acc_mean_calc(sum_y);
    acc_mean.z = acc_mean_calc(sum_z);

    return acc_mean;
}

vector3_t acc_ref_get(circBuf_t *x_buff, circBuf_t *y_buff, circBuf_t *z_buff, uint8_t startup)
{
    uint16_t i;
    int32_t sum_x = 0;
    int32_t sum_y = 0;
    int32_t sum_z = 0;

    //Reference position
    vector3_t ref;

    if (startup) {
        for (i = 0; i < ACC_BUF_SIZE; i++)
        {
            acc_buff_write(x_buff, y_buff, z_buff);
            SysCtlDelay(SysCtlClockGet() / 100);
        }
    }

    for (i = 0; i < ACC_BUF_SIZE; i++) {
        sum_x = sum_x + readCircBuf(x_buff);
        sum_y = sum_y + readCircBuf(y_buff);
        sum_z = sum_z + readCircBuf(z_buff);
    }

    ref.x = acc_mean_calc(sum_x);
    ref.y = acc_mean_calc(sum_y);
    ref.z = acc_mean_calc(sum_z);

    return ref;
}
