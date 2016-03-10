/*******************************************************************************
* File Name: EZI2C_EZI2C_BOOT.c
* Version 3.10
*
* Description:
*  This file provides the source code of the bootloader communication APIs
*  for the SCB Component EZI2C mode.
*
* Note:
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "EZI2C_BOOT.h"
#include "EZI2C_EZI2C_PVT.h"

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (EZI2C_EZI2C_BTLDR_COMM_ENABLED)

/***************************************
*    Private I/O Component Vars
***************************************/


/*******************************************************************************
* Function Name: EZI2C_EzI2CCyBtldrCommStart
********************************************************************************
*
* Summary:
*  This function does nothing.
*  The SCB in EZI2C mode does not support the bootloader communication.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void EZI2C_EzI2CCyBtldrCommStart(void)
{
    /* Do nothing */
}


/*******************************************************************************
* Function Name: EZI2C_EzI2CCyBtldrCommStop
********************************************************************************
*
* Summary:
*  This function does nothing.
*  The SCB in the EZI2C mode does not support the bootloader communication.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void EZI2C_EzI2CCyBtldrCommStop(void)
{
    /* Do nothing */
}


/*******************************************************************************
* Function Name: EZI2C_EzI2CCyBtldrCommReset
********************************************************************************
*
* Summary:
*  This function does nothing.
*  The SCB in the EZI2C mode does not support the bootloader communication.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void EZI2C_EzI2CCyBtldrCommReset(void)
{
    /* Do nothing */
}


/*******************************************************************************
* Function Name: EZI2C_EzI2CCyBtldrCommRead
********************************************************************************
*
* Summary:
*  This function does nothing.
*  The SCB in the EZI2C mode does not support the bootloader communication.
*
* Parameters:
*  pData:    Pointer to storage for the block of data to be read from the
*            bootloader host
*  size:     Number of bytes to be read.
*  count:    Pointer to the variable to write the number of bytes actually
*            read.
*  timeOut:  Number of units in 10 ms to wait before returning because of a
*            timeout.
*
* Return:
*  Returns CYRET_SUCCESS if no problem was encountered or returns the value
*  that best describes the problem.
*
*******************************************************************************/
cystatus EZI2C_EzI2CCyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;
    status = CYRET_BAD_PARAM;

    return(status);
}


/*******************************************************************************
* Function Name: EZI2C_EzI2CCyBtldrCommWrite
********************************************************************************
*
* Summary:
*  This function does nothing.
*  The SCB in the EZI2C mode does not support the bootloader communication.
*
* Parameters:
*  pData:    Pointer to the block of data to be written to the bootloader host.
*  size:     Number of bytes to be written.
*  count:    Pointer to the variable to write the number of bytes actually
*            written.
*  timeOut:  Number of units in 10 ms to wait before returning because of a
*            timeout.
*
* Return:
*  Returns CYRET_SUCCESS if no problem was encountered or returns the value
*  that best describes the problem.
*
*******************************************************************************/
cystatus EZI2C_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;
    status = CYRET_BAD_PARAM;

    return(status);
}


#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (EZI2C_EZI2C_BTLDR_COMM_ENABLED) */


/* [] END OF FILE */
