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


int Board_Main()
{       
    /* Start this right away so that we debug as soon as possible */
    Debug_Init();
    Debug_Start();
        
    Control_Init();
    Time_Init();
    I2c_Init();
    Encoder_Init();
    Motor_Init();

    Time_Start();
    I2c_Start();
    PID_Start();
    Encoder_Start();    
    Motor_Start();
    
    Control_Start();

    for(;;)
    {
        /* Apply the velocity command to PID */
        PID_Update();
        /* Update the encoder calculation */
        Encoder_Update();
        /* Read the infrared sensors */
        Infrared_Measure();
        /* Read the ultrasonic sensors */
        Ultrasonic_Measure();
        /* Update any control changes */
        Control_Update();
    }
}

/* [] END OF FILE */
