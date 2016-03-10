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

#include "utils.h"

uint32 MovingAverage(MOVING_AVERAGE_TYPE* ma, uint32 value)
/*
MA*[i]= MA*[i-1] +X[i] - MA*[i-1]/N

where MA* is the moving average*N. 

MA[i]= MA*[i]/N
*/
{
    uint32 ma_curr;
    
    ma_curr = ma->last + value - ma->last/ma->n;
    ma->last = ma_curr;
    
    return ma_curr/ma->n;
}


/* [] END OF FILE */
