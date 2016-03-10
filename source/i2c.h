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

#ifndef I2C_H
#define I2C_H

#include <project.h>

void I2c_Init();
void I2c_Start();

uint16 I2c_ReadControl();
void I2c_ReadVelocity(int16* velocity_percent);
void I2c_ReadAcceleration(int16* accel_percent);
void I2c_WriteStatus(uint16 mask);
void I2c_WriteCount(uint32 count);
void I2c_WriteVelocity(int16 velocity_percent);
void I2c_WriteUltrasonicDistance(uint8 offset, uint16 distance);
void I2c_WriteInfraredDistance(uint8 offset, uint8 distance);
void I2c_WriteTest();

#endif

/* [] END OF FILE */
