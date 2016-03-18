/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "i2c.h"
#include "time.h"
#include "hwconfig.h"

/*
    Buffer Layout

    offset     num bytes     name                           description
      00           2         [control register]             the control register supports 
                                                            enable/disable of the HB25 motors and
                                                            Encoder counts
      02           2         [commanded velocity]           commanded velocity is in units of 
                                                            millimeters/second
      04           2         [commanded accel]              commanded velocity in in units of 
                                                            millimeters/second^2
    ------------------------------ Read/Write Boundary --------------------------------------------
      06           2         [device status]                contains bits that represent the status of the Psoc device
                                                               - HB25 Motor Controller Initialize b0000000000000001
      08           4         [measured count]               measured count is the encoder count
      12           2         [measured velocity]            measured velocity is the measured speed 
                                                            of the wheel using the encoder count
      14          16         [ultrasonic distance]          ultrasonic distance is an array of 
                                                            distances from the ultrasonic sensors in 
                                                            centimeters, range 2 to 500
      30           8         [infrared distance]            infrared distance is an array of distances 
                                                            from the infrared sensors in centimeters, 
                                                            range 10 to 80
      38           1         [test]                         used for testing the i2c communication
      39        [empty]
 */

#define I2C_BUFFER_BASE (0)
#define I2C_READ_WRITE_OFFSET (I2C_BUFFER_BASE)

#define CONTROL_BASE (I2C_READ_WRITE_OFFSET)
#define CONTROL_SIZE (sizeof(uint16))

#define COMMANDED_VELOCITY_BASE (CONTROL_BASE + CONTROL_SIZE)
#define COMMANDED_VELOCITY_SIZE (sizeof(uint16))

#define COMMANDED_ACCELERATION_BASE (COMMANDED_VELOCITY_BASE + COMMANDED_VELOCITY_SIZE)
#define COMMANDED_ACCELERATION_SIZE (sizeof(uint16))

/* The read/write size is the number of bytes (starting from the beginning of the buffer
   that the master can write to.  The master can always read from the entire buffer
 */
#define I2C_READ_WRITE_SIZE (CONTROL_SIZE + COMMANDED_VELOCITY_SIZE + COMMANDED_ACCELERATION_SIZE)
#define I2C_READ_ONLY_OFFSET (I2C_READ_WRITE_OFFSET + I2C_READ_WRITE_SIZE)

#define DEVICE_STATUS_BASE (I2C_READ_ONLY_OFFSET)
#define DEVICE_STATUS_SIZE (sizeof(uint16))

#define MEASURED_COUNT_BASE (DEVICE_STATUS_BASE + DEVICE_STATUS_SIZE)
#define MEASURED_COUNT_SIZE (sizeof(uint32))

#define MEASURED_VELOCITY_BASE (MEASURED_COUNT_BASE + MEASURED_COUNT_SIZE)
#define MEASURED_VELOCITY_SIZE (sizeof(uint16))

#define ULTRASONIC_DISTANCE_BASE (MEASURED_VELOCITY_BASE + MEASURED_VELOCITY_SIZE)
#define ULTRASONIC_DISTANCE_SIZE (NUM_ULTRASONIC_SENSORS * sizeof(uint16))

#define INFRARED_DISTANCE_BASE (ULTRASONIC_DISTANCE_BASE + ULTRASONIC_DISTANCE_SIZE)
#define INFRARED_DISTANCE_SIZE (NUM_INFRARED_SENSORS * sizeof(uint8))

#define TEST_BASE (ULTRASONIC_DISTANCE_BASE + ULTRASONIC_DISTANCE_SIZE)
#define TEST_SIZE (1)

#define I2C_READ_ONLY_SIZE (MEASURED_COUNT_SIZE + MEASURED_VELOCITY_SIZE + ULTRASONIC_DISTANCE_SIZE + INFRARED_DISTANCE_SIZE + TEST_SIZE)

#define BUFFER_SIZE (I2C_READ_WRITE_SIZE + I2C_READ_ONLY_SIZE)

static uint8 i2c_buf[BUFFER_SIZE];


void I2c_Init()
{
    memset( i2c_buf, 0, sizeof(i2c_buf));
}

void I2c_Start()
{
    EZI2C_EzI2CSetAddress1(I2C_ADDRESS);
    EZI2C_Start();
    
    EZI2C_EzI2CSetBuffer1(sizeof(i2c_buf), I2C_READ_WRITE_SIZE, i2c_buf);
}

uint16 I2c_ReadControl()
{
    return (uint16) (i2c_buf[CONTROL_BASE + 1] << 8) | i2c_buf[CONTROL_BASE];
}

void I2c_WriteStatus(uint16 mask)
{
    i2c_buf[DEVICE_STATUS_BASE] |= (mask >> 8);
    i2c_buf[DEVICE_STATUS_BASE + 1] |= (mask & 0xFF);
}

void I2c_ReadVelocity(int16* velocity_percent)
{
    *velocity_percent = (int16) (i2c_buf[COMMANDED_VELOCITY_BASE + 1] << 8) | i2c_buf[COMMANDED_VELOCITY_BASE];
}

void I2c_ReadAcceleration(int16* accel_percent)
{
    *accel_percent = (int16) (i2c_buf[COMMANDED_ACCELERATION_BASE + 1] << 8) | i2c_buf[COMMANDED_ACCELERATION_BASE];
}

void I2c_WriteCount(uint32 count)
{
    i2c_buf[MEASURED_COUNT_BASE] = (uint8) (count >> 24) & 0x000000ff;
    i2c_buf[MEASURED_COUNT_BASE + 1] = (uint8) (count >> 16) & 0x000000ff;
    i2c_buf[MEASURED_COUNT_BASE + 2] = (uint8) (count >> 8) & 0x000000ff;
    i2c_buf[MEASURED_COUNT_BASE + 3] = (uint8) (count & 0x000000ff);
}

void I2c_WriteVelocity(int16 velocity_percent)
{
    i2c_buf[MEASURED_VELOCITY_BASE] = (uint8) velocity_percent & 0x00ff;
    i2c_buf[MEASURED_VELOCITY_BASE + 1] = (uint8) (velocity_percent >> 8) & 0x00ff;
}

void I2c_WriteUltrasonicDistance(uint8 offset, uint16 distance)
{
    uint8 index = offset * sizeof(distance);
    i2c_buf[ULTRASONIC_DISTANCE_BASE + index] = (distance >> 8) & 0x00ff;
    i2c_buf[ULTRASONIC_DISTANCE_BASE + index + 1] = distance & 0x00ff;
}

void I2c_WriteInfraredDistance(uint8 offset, uint8 distance)
{
    i2c_buf[INFRARED_DISTANCE_BASE + offset] = distance;
}

void I2c_WriteTest()
{
    static uint32 last_time = 0;
    static uint8 test_count = 1;
    
    if (millis() - last_time > 1000)
    {
        last_time = millis();
        i2c_buf[TEST_BASE] = test_count++;
    }
}

/* [] END OF FILE */
