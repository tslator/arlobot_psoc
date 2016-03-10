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

#include "control.h"
#include "i2c.h"
#include "encoder.h"
#include "motor.h"
#include "time.h"

/* The purpose of this module is to handle control changes to the system.
   The only thing considered at this point is shutting down the motors properly.
 */

#define POWER_ON_MOTOR (0x0001)
#define SPEED_TIMEOUT  (2000) // millisecond

static uint32 last_time;

void Control_Init()
{
    last_time = millis();
}

void Control_Start()
{    
    /* Note: I'm not sure what to do here, was thinking that this could be a way to enabled/disabled features at startup */
    uint16 control = I2c_ReadControl();
}

void Control_Update()
{
    uint8 motor_state;
    uint16 control;
    
    control = I2c_ReadControl();
    
    /* If a speed command hasn't been received within the given threshold, stop the motor
        
       Recommendation from Parallax:
        The HB-25 has a selectable Timeout mode. When this mode is enabled, the HB-25 will shut off the motor
        after 4 seconds if it does not receive pulses from the microcontroller. Once the motor is shut off, if the
        HB-25 receives a valid pulse again, it will restart the motor. Timeout mode should always be enabled in
        applications where a failure of your microcontroller could cause a dangerous situation or safety risk, or
        potentially cause damage to your application. 
     */
    if ((millis() - last_time) > SPEED_TIMEOUT)
    {
        Motor_SetOutput(0);
    }
    
    /* Along the same lines as Control_Start() this could be a way to disable or shutdown features, like turn off the 
       motor drivers or the motors via a relay */
    
    motor_state = control & POWER_ON_MOTOR;
    if ( motor_state )
    {
        Motor_Start();
    }
    else
    {
        Motor_Stop();
    }
    
}

/* [] END OF FILE */
