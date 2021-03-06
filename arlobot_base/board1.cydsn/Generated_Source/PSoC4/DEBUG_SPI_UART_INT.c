/*******************************************************************************
* File Name: DEBUG_SPI_UART_INT.c
* Version 3.10
*
* Description:
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in SPI and UART modes.
*
* Note:
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "DEBUG_PVT.h"
#include "DEBUG_SPI_UART_PVT.h"
#include "cyapicallbacks.h"

#if (DEBUG_SCB_IRQ_INTERNAL)
/*******************************************************************************
* Function Name: DEBUG_SPI_UART_ISR
********************************************************************************
*
* Summary:
*  Handles the Interrupt Service Routine for the SCB SPI or UART modes.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
CY_ISR(DEBUG_SPI_UART_ISR)
{
#if (DEBUG_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
#endif /* (DEBUG_INTERNAL_RX_SW_BUFFER_CONST) */

#if (DEBUG_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (DEBUG_INTERNAL_TX_SW_BUFFER_CONST) */

#ifdef DEBUG_SPI_UART_ISR_ENTRY_CALLBACK
    DEBUG_SPI_UART_ISR_EntryCallback();
#endif /* DEBUG_SPI_UART_ISR_ENTRY_CALLBACK */

    if (NULL != DEBUG_customIntrHandler)
    {
        DEBUG_customIntrHandler();
    }

    #if (DEBUG_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        DEBUG_ClearSpiExtClkInterruptSource(DEBUG_INTR_SPI_EC_WAKE_UP);
    }
    #endif

    #if (DEBUG_CHECK_RX_SW_BUFFER)
    {
        if (DEBUG_CHECK_INTR_RX_MASKED(DEBUG_INTR_RX_NOT_EMPTY))
        {
            do
            {
                /* Move local head index */
                locHead = (DEBUG_rxBufferHead + 1u);

                /* Adjust local head index */
                if (DEBUG_INTERNAL_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if (locHead == DEBUG_rxBufferTail)
                {
                    #if (DEBUG_CHECK_UART_RTS_CONTROL_FLOW)
                    {
                        /* There is no space in the software buffer - disable the
                        * RX Not Empty interrupt source. The data elements are
                        * still being received into the RX FIFO until the RTS signal
                        * stops the transmitter. After the data element is read from the
                        * buffer, the RX Not Empty interrupt source is enabled to
                        * move the next data element in the software buffer.
                        */
                        DEBUG_INTR_RX_MASK_REG &= ~DEBUG_INTR_RX_NOT_EMPTY;
                        break;
                    }
                    #else
                    {
                        /* Overflow: through away received data element */
                        (void) DEBUG_RX_FIFO_RD_REG;
                        DEBUG_rxBufferOverflow = (uint8) DEBUG_INTR_RX_OVERFLOW;
                    }
                    #endif
                }
                else
                {
                    /* Store received data */
                    DEBUG_PutWordInRxBuffer(locHead, DEBUG_RX_FIFO_RD_REG);

                    /* Move head index */
                    DEBUG_rxBufferHead = locHead;
                }
            }
            while(0u != DEBUG_GET_RX_FIFO_ENTRIES);

            DEBUG_ClearRxInterruptSource(DEBUG_INTR_RX_NOT_EMPTY);
        }
    }
    #endif


    #if (DEBUG_CHECK_TX_SW_BUFFER)
    {
        if (DEBUG_CHECK_INTR_TX_MASKED(DEBUG_INTR_TX_NOT_FULL))
        {
            do
            {
                /* Check for room in TX software buffer */
                if (DEBUG_txBufferHead != DEBUG_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (DEBUG_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if (DEBUG_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    DEBUG_TX_FIFO_WR_REG = DEBUG_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    DEBUG_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    DEBUG_DISABLE_INTR_TX(DEBUG_INTR_TX_NOT_FULL);
                    break;
                }
            }
            while (DEBUG_SPI_UART_FIFO_SIZE != DEBUG_GET_TX_FIFO_ENTRIES);

            DEBUG_ClearTxInterruptSource(DEBUG_INTR_TX_NOT_FULL);
        }
    }
    #endif
    
#ifdef DEBUG_SPI_UART_ISR_EXIT_CALLBACK
    DEBUG_SPI_UART_ISR_ExitCallback();
#endif /* DEBUG_SPI_UART_ISR_EXIT_CALLBACK */
    
}

#endif /* (DEBUG_SCB_IRQ_INTERNAL) */


/* [] END OF FILE */
