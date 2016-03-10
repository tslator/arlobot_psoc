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

#include <math.h>
#include "pid.h"
#include "encoder.h"
#include "motor.h"
#include "i2c.h"
#include "utils.h"
#include "debug.h"
#include "time.h"

#define SAMPLE_RATE (20) // Hz
#define SAMPLE_PERIOD (1000/SAMPLE_RATE) /* milliseconds */

#define MIN_KP (0)
#define MAX_KP (1)
#define MIN_KI (0)
#define MAX_KI (1)
#define MIN_KD (0)
#define MAX_KD (1)
#define MIN_SAMPLE_RATE (0)
#define MAX_SAMPLE_RATE (1)

#define DEFAULT_KP (5.0)
#define DEFAULT_KI (0.1)
#define DEFAULT_KD (0.01)

#define MAX_VELOCITY (1500)
#define MIN_ERROR (0.001)

static float Kp;
static float Ki;
static float Kd;

/*---------------------------------*/


static int8 Calculate(int16 set_velocity, int16 calc_velocity)
{
    static float prev_error = 0;
    static float integral = 0;
    float scaled_calc_velocity;
    float scaled_set_velocity;
    float error;
    float derivative;
    float output;
    int8 scaled_output;
    
    scaled_calc_velocity = calc_velocity/MAX_VELOCITY; //scale(calc_velocity, 100, 15000);
    scaled_set_velocity = set_velocity/MAX_VELOCITY; //scale(set_velocity, 100, 15000);
    
    COMMS_DEBUG_PRINT(0, "CV: %d, SCV: %f, SV: %d, SSV: %f\n\r", calc_velocity, scaled_calc_velocity, set_velocity, scaled_set_velocity);

    error = scaled_set_velocity - scaled_calc_velocity;

    if( fabs(error) > MIN_ERROR )
    {
        integral = integral + error;
    }

    derivative = (error - prev_error);
    output = Kp * error + Ki * integral + Kd * derivative;
    
    scaled_output = (int8) constrain((output * 100), -100, 100);//constrain(output, -100, 100);
    
    prev_error = error;
    
    return scaled_output;
}
    
void PID_Init(float p, float i, float d, uint8 sample_rate)
{
    Kp = p;
    Ki = i;
    Kd = d;
    sample_rate = constrain(sample_rate, MIN_SAMPLE_RATE, MAX_SAMPLE_RATE);
}

void PID_Start()
{
    Kp = DEFAULT_KP;
    Ki = DEFAULT_KI;
    Kd = DEFAULT_KD;
}

void PID_Update(int16 velocity)
{
    static uint32 last_time = 0;
    int16 calc_velocity;
    int8 new_set_velocity;
    
    if (DELTA_TIME(millis(), last_time, SAMPLE_PERIOD))
    {    
        last_time = millis();
        
        calc_velocity = Encoder_GetVelocity();
        new_set_velocity = Calculate(velocity, calc_velocity);
        Motor_SetOutput(new_set_velocity);
    }
}


/* [] END OF FILE */
