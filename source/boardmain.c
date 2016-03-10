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
#include <stdio.h>
#include <project.h>
#include "control.h"
#include "i2c.h"
#include "pid.h"
#include "encoder.h"
#include "motor.h"
#include "ultrasonic.h"
#include "infrared.h"
#include "time.h"
#include "debug.h"

#define MAX_MILLIMETERS_PER_SECOND (1000)
#define MIN_MILLIMETERS_PER_SECOND (-1000)

static int16 velocity_cmd;
static uint32 meas_count;
static uint16 meas_velocity;

int Board_Main()
{       
    /* Start this right away so that we debug as soon as possible */
    Debug_Init();
    //Debug_Start();
        
    Control_Init();
    Time_Init();
    I2c_Init();
    Encoder_Init();
    Motor_Init();

    Time_Start();
    I2c_Start();
    Encoder_Start();
    Motor_Start();
    
    Control_Start();

    for(;;)
    {
#ifdef MOTOR_TEST
        MotorTestDrive();
#endif
        
        I2c_ReadVelocity(&velocity_cmd);
        
        PID_Update(velocity_cmd);
        Encoder_Update();
        
        meas_count = Encoder_GetCount();
        meas_velocity = Encoder_GetVelocity();
        
        COMMS_DEBUG_PRINT(0, "CV: %d, EC: %ld, MV: %d\n\r", velocity_cmd, meas_count, meas_velocity);
        
        I2c_WriteCount(meas_count);
        I2c_WriteVelocity(meas_velocity);

        Infrared_Measure();
        Ultrasonic_Measure();
        
        Control_Update();
        
        I2c_WriteTest();
    }
}

#ifdef MOTOR_TEST
static void MotorTestDrive()
{
    #define ZERO_TO_MAX_POS (1)
    #define MAX_POS_TO_MAX_NEG (2)
    #define MAX_NEG_TO_ZERO (3)
    
    static uint8 stage = ZERO_TO_MAX_POS;
    
    uint32 now;

    now = millis();
    if (DELTA_TIME(now, last_time, 50))
    {
        last_time = now;
        
        switch (stage)
        {
            case ZERO_TO_MAX_POS:
                velocity_cmd += 100;
                if (velocity_cmd == MAX_MILLIMETERS_PER_SECOND)
                {
                    stage = MAX_POS_TO_MAX_NEG;
                }
                break;
                
            case MAX_POS_TO_MAX_NEG:
                velocity_cmd -= 100;
                if (velocity_cmd == MIN_MILLIMETERS_PER_SECOND)
                {
                    stage = MAX_NEG_TO_ZERO;
                }
                break;
                
            case MAX_NEG_TO_ZERO:
                velocity_cmd += 100;
                if (velocity_cmd == 0)
                {
                    stage = ZERO_TO_MAX_POS;
                }
                break;
        }
        
        DEBUG_PRINT(0, "Target Velocity: %d\n\r", velocity_cmd);
    }

    Encoder_Update();
    PID_Update(velocity_cmd);
}
#endif
/* [] END OF FILE */
