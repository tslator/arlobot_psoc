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
#include <project.h>

/* The source code is stored outside of the Psoc project in a relative directory so that is can be shared with other 
   projects if necessary.  The main loop is implemented in the Board_Main function found in the boardmain.c file
 */
extern void Board_Main();

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    Board_Main();
    
    /* Will never happen because Board_Main never returns, but it makes the compiler happy :-P */
    return 0;
}

/* [] END OF FILE */
