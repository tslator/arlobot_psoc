/*******************************************************************************
* File Name: UARTCOMMS_SPI_UART.c
* Version 3.10
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  SPI and UART modes.
*
* Note:
*
*******************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UARTCOMMS_PVT.h"
#include "UARTCOMMS_SPI_UART_PVT.h"

/***************************************
*        SPI/UART Private Vars
***************************************/

#if(UARTCOMMS_INTERNAL_RX_SW_BUFFER_CONST)
    volatile uint32 UARTCOMMS_rxBufferHead;
    volatile uint32 UARTCOMMS_rxBufferTail;
    volatile uint8  UARTCOMMS_rxBufferOverflow;
#endif /* (UARTCOMMS_INTERNAL_RX_SW_BUFFER_CONST) */

#if(UARTCOMMS_INTERNAL_TX_SW_BUFFER_CONST)
    volatile uint32 UARTCOMMS_txBufferHead;
    volatile uint32 UARTCOMMS_txBufferTail;
#endif /* (UARTCOMMS_INTERNAL_TX_SW_BUFFER_CONST) */

#if(UARTCOMMS_INTERNAL_RX_SW_BUFFER)
    /* Add one element to the buffer to receive full packet. One byte in receive buffer is always empty */
    volatile uint8 UARTCOMMS_rxBufferInternal[UARTCOMMS_INTERNAL_RX_BUFFER_SIZE];
#endif /* (UARTCOMMS_INTERNAL_RX_SW_BUFFER) */

#if(UARTCOMMS_INTERNAL_TX_SW_BUFFER)
    volatile uint8 UARTCOMMS_txBufferInternal[UARTCOMMS_TX_BUFFER_SIZE];
#endif /* (UARTCOMMS_INTERNAL_TX_SW_BUFFER) */


#if(UARTCOMMS_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: UARTCOMMS_SpiUartReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Retrieves the next data element from the receive buffer.
    *   - RX software buffer is disabled: Returns data element retrieved from
    *     RX FIFO. Undefined data will be returned if the RX FIFO is empty.
    *   - RX software buffer is enabled: Returns data element from the software
    *     receive buffer. Zero value is returned if the software receive buffer
    *     is empty.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Next data element from the receive buffer.
    *
    * Global Variables:
    *  Look into UARTCOMMS_SpiInit for description.
    *
    *******************************************************************************/
    uint32 UARTCOMMS_SpiUartReadRxData(void)
    {
        uint32 rxData = 0u;

    #if (UARTCOMMS_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (UARTCOMMS_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (UARTCOMMS_CHECK_RX_SW_BUFFER)
        {
            if (UARTCOMMS_rxBufferHead != UARTCOMMS_rxBufferTail)
            {
                /* There is data in RX software buffer */

                /* Calculate index to read from */
                locTail = (UARTCOMMS_rxBufferTail + 1u);

                if (UARTCOMMS_INTERNAL_RX_BUFFER_SIZE == locTail)
                {
                    locTail = 0u;
                }

                /* Get data from RX software buffer */
                rxData = UARTCOMMS_GetWordFromRxBuffer(locTail);

                /* Change index in the buffer */
                UARTCOMMS_rxBufferTail = locTail;

                #if (UARTCOMMS_CHECK_UART_RTS_CONTROL_FLOW)
                {
                    /* Check if RX Not Empty is disabled in the interrupt */
                    if (0u == (UARTCOMMS_INTR_RX_MASK_REG & UARTCOMMS_INTR_RX_NOT_EMPTY))
                    {
                        /* Enable RX Not Empty interrupt source to continue
                        * receiving data into software buffer.
                        */
                        UARTCOMMS_INTR_RX_MASK_REG |= UARTCOMMS_INTR_RX_NOT_EMPTY;
                    }
                }
                #endif

            }
        }
        #else
        {
            /* Read data from RX FIFO */
            rxData = UARTCOMMS_RX_FIFO_RD_REG;
        }
        #endif

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: UARTCOMMS_SpiUartGetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received data elements in the receive buffer.
    *   - RX software buffer disabled: returns the number of used entries in
    *     RX FIFO.
    *   - RX software buffer enabled: returns the number of elements which were
    *     placed in the receive buffer. This does not include the hardware RX FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Number of received data elements
    *
    *******************************************************************************/
    uint32 UARTCOMMS_SpiUartGetRxBufferSize(void)
    {
        uint32 size;
    #if (UARTCOMMS_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (UARTCOMMS_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (UARTCOMMS_CHECK_RX_SW_BUFFER)
        {
            locHead = UARTCOMMS_rxBufferHead;

            if(locHead >= UARTCOMMS_rxBufferTail)
            {
                size = (locHead - UARTCOMMS_rxBufferTail);
            }
            else
            {
                size = (locHead + (UARTCOMMS_INTERNAL_RX_BUFFER_SIZE - UARTCOMMS_rxBufferTail));
            }
        }
        #else
        {
            size = UARTCOMMS_GET_RX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: UARTCOMMS_SpiUartClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receive buffer and RX FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void UARTCOMMS_SpiUartClearRxBuffer(void)
    {
        #if (UARTCOMMS_CHECK_RX_SW_BUFFER)
        {
            /* Lock from component interruption */
            UARTCOMMS_DisableInt();

            /* Flush RX software buffer */
            UARTCOMMS_rxBufferHead = UARTCOMMS_rxBufferTail;
            UARTCOMMS_rxBufferOverflow = 0u;

            UARTCOMMS_CLEAR_RX_FIFO;
            UARTCOMMS_ClearRxInterruptSource(UARTCOMMS_INTR_RX_ALL);

            #if (UARTCOMMS_CHECK_UART_RTS_CONTROL_FLOW)
            {
                /* Enable RX Not Empty interrupt source to continue receiving
                * data into software buffer.
                */
                UARTCOMMS_INTR_RX_MASK_REG |= UARTCOMMS_INTR_RX_NOT_EMPTY;
            }
            #endif

            /* Release lock */
            UARTCOMMS_EnableInt();
        }
        #else
        {
            UARTCOMMS_CLEAR_RX_FIFO;
        }
        #endif
    }

#endif /* (UARTCOMMS_RX_DIRECTION) */


#if(UARTCOMMS_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: UARTCOMMS_SpiUartWriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a data entry into the transmit buffer to be sent at the next available
    *  bus time.
    *  This function is blocking and waits until there is space available to put the
    *  requested data in the transmit buffer.
    *
    * Parameters:
    *  txDataByte: the data to be transmitted.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void UARTCOMMS_SpiUartWriteTxData(uint32 txData)
    {
    #if (UARTCOMMS_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (UARTCOMMS_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (UARTCOMMS_CHECK_TX_SW_BUFFER)
        {
            /* Put data directly into the TX FIFO */
            if ((UARTCOMMS_txBufferHead == UARTCOMMS_txBufferTail) &&
                (UARTCOMMS_SPI_UART_FIFO_SIZE != UARTCOMMS_GET_TX_FIFO_ENTRIES))
            {
                /* TX software buffer is empty: put data directly in TX FIFO */
                UARTCOMMS_TX_FIFO_WR_REG = txData;
            }
            /* Put data into TX software buffer */
            else
            {
                /* Head index to put data */
                locHead = (UARTCOMMS_txBufferHead + 1u);

                /* Adjust TX software buffer index */
                if (UARTCOMMS_TX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                /* Wait for space in TX software buffer */
                while (locHead == UARTCOMMS_txBufferTail)
                {
                }

                /* TX software buffer has at least one room */

                /* Clear old status of INTR_TX_NOT_FULL. It sets at the end of transfer when TX FIFO is empty. */
                UARTCOMMS_ClearTxInterruptSource(UARTCOMMS_INTR_TX_NOT_FULL);

                UARTCOMMS_PutWordInTxBuffer(locHead, txData);

                UARTCOMMS_txBufferHead = locHead;

                /* Check if TX Not Full is disabled in interrupt */
                if (0u == (UARTCOMMS_INTR_TX_MASK_REG & UARTCOMMS_INTR_TX_NOT_FULL))
                {
                    /* Enable TX Not Full interrupt source to transmit from software buffer */
                    UARTCOMMS_INTR_TX_MASK_REG |= (uint32) UARTCOMMS_INTR_TX_NOT_FULL;
                }
            }
        }
        #else
        {
            /* Wait until TX FIFO has space to put data element */
            while (UARTCOMMS_SPI_UART_FIFO_SIZE == UARTCOMMS_GET_TX_FIFO_ENTRIES)
            {
            }

            UARTCOMMS_TX_FIFO_WR_REG = txData;
        }
        #endif
    }


    /*******************************************************************************
    * Function Name: UARTCOMMS_SpiUartPutArray
    ********************************************************************************
    *
    * Summary:
    *  Places an array of data into the transmit buffer to be sent.
    *  This function is blocking and waits until there is a space available to put
    *  all the requested data in the transmit buffer. The array size can be greater
    *  than transmit buffer size.
    *
    * Parameters:
    *  wrBuf:  pointer to an array with data to be placed in transmit buffer.
    *  count:  number of data elements to be placed in the transmit buffer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void UARTCOMMS_SpiUartPutArray(const uint8 wrBuf[], uint32 count)
    {
        uint32 i;

        for (i=0u; i < count; i++)
        {
            UARTCOMMS_SpiUartWriteTxData((uint32) wrBuf[i]);
        }
    }


    /*******************************************************************************
    * Function Name: UARTCOMMS_SpiUartGetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    * Returns the number of elements currently in the transmit buffer.
    *  - TX software buffer is disabled: returns the number of used entries in
    *    TX FIFO.
    *  - TX software buffer is enabled: returns the number of elements currently
    *    used in the transmit buffer. This number does not include used entries in
    *    the TX FIFO. The transmit buffer size is zero until the TX FIFO is
    *    not full.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Number of data elements ready to transmit.
    *
    *******************************************************************************/
    uint32 UARTCOMMS_SpiUartGetTxBufferSize(void)
    {
        uint32 size;
    #if (UARTCOMMS_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (UARTCOMMS_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (UARTCOMMS_CHECK_TX_SW_BUFFER)
        {
            /* Get current Tail index */
            locTail = UARTCOMMS_txBufferTail;

            if (UARTCOMMS_txBufferHead >= locTail)
            {
                size = (UARTCOMMS_txBufferHead - locTail);
            }
            else
            {
                size = (UARTCOMMS_txBufferHead + (UARTCOMMS_TX_BUFFER_SIZE - locTail));
            }
        }
        #else
        {
            size = UARTCOMMS_GET_TX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: UARTCOMMS_SpiUartClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the transmit buffer and TX FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void UARTCOMMS_SpiUartClearTxBuffer(void)
    {
        #if (UARTCOMMS_CHECK_TX_SW_BUFFER)
        {
            /* Lock from component interruption */
            UARTCOMMS_DisableInt();

            /* Flush TX software buffer */
            UARTCOMMS_txBufferHead = UARTCOMMS_txBufferTail;

            UARTCOMMS_INTR_TX_MASK_REG &= (uint32) ~UARTCOMMS_INTR_TX_NOT_FULL;
            UARTCOMMS_CLEAR_TX_FIFO;
            UARTCOMMS_ClearTxInterruptSource(UARTCOMMS_INTR_TX_ALL);

            /* Release lock */
            UARTCOMMS_EnableInt();
        }
        #else
        {
            UARTCOMMS_CLEAR_TX_FIFO;
        }
        #endif
    }

#endif /* (UARTCOMMS_TX_DIRECTION) */


/*******************************************************************************
* Function Name: UARTCOMMS_SpiUartDisableIntRx
********************************************************************************
*
* Summary:
*  Disables the RX interrupt sources.
*
* Parameters:
*  None
*
* Return:
*  Returns the RX interrupt sources enabled before the function call.
*
*******************************************************************************/
uint32 UARTCOMMS_SpiUartDisableIntRx(void)
{
    uint32 intSource;

    intSource = UARTCOMMS_GetRxInterruptMode();

    UARTCOMMS_SetRxInterruptMode(UARTCOMMS_NO_INTR_SOURCES);

    return (intSource);
}


/*******************************************************************************
* Function Name: UARTCOMMS_SpiUartDisableIntTx
********************************************************************************
*
* Summary:
*  Disables TX interrupt sources.
*
* Parameters:
*  None
*
* Return:
*  Returns TX interrupt sources enabled before function call.
*
*******************************************************************************/
uint32 UARTCOMMS_SpiUartDisableIntTx(void)
{
    uint32 intSourceMask;

    intSourceMask = UARTCOMMS_GetTxInterruptMode();

    UARTCOMMS_SetTxInterruptMode(UARTCOMMS_NO_INTR_SOURCES);

    return (intSourceMask);
}


#if(UARTCOMMS_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: UARTCOMMS_PutWordInRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Stores a byte/word into the RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    * Parameters:
    *  index:      index to store data byte/word in the RX buffer.
    *  rxDataByte: byte/word to store.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void UARTCOMMS_PutWordInRxBuffer(uint32 idx, uint32 rxDataByte)
    {
        /* Put data in buffer */
        if (UARTCOMMS_ONE_BYTE_WIDTH == UARTCOMMS_rxDataBits)
        {
            UARTCOMMS_rxBuffer[idx] = ((uint8) rxDataByte);
        }
        else
        {
            UARTCOMMS_rxBuffer[(uint32)(idx << 1u)]      = LO8(LO16(rxDataByte));
            UARTCOMMS_rxBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(rxDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: UARTCOMMS_GetWordFromRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Reads byte/word from RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Returns byte/word read from RX buffer.
    *
    *******************************************************************************/
    uint32 UARTCOMMS_GetWordFromRxBuffer(uint32 idx)
    {
        uint32 value;

        if (UARTCOMMS_ONE_BYTE_WIDTH == UARTCOMMS_rxDataBits)
        {
            value = UARTCOMMS_rxBuffer[idx];
        }
        else
        {
            value  = (uint32) UARTCOMMS_rxBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32)UARTCOMMS_rxBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }


    /*******************************************************************************
    * Function Name: UARTCOMMS_PutWordInTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Stores byte/word into the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    * Parameters:
    *  idx:        index to store data byte/word in the TX buffer.
    *  txDataByte: byte/word to store.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void UARTCOMMS_PutWordInTxBuffer(uint32 idx, uint32 txDataByte)
    {
        /* Put data in buffer */
        if (UARTCOMMS_ONE_BYTE_WIDTH == UARTCOMMS_txDataBits)
        {
            UARTCOMMS_txBuffer[idx] = ((uint8) txDataByte);
        }
        else
        {
            UARTCOMMS_txBuffer[(uint32)(idx << 1u)]      = LO8(LO16(txDataByte));
            UARTCOMMS_txBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(txDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: UARTCOMMS_GetWordFromTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Reads byte/word from the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    * Parameters:
    *  idx: index to get data byte/word from the TX buffer.
    *
    * Return:
    *  Returns byte/word read from the TX buffer.
    *
    *******************************************************************************/
    uint32 UARTCOMMS_GetWordFromTxBuffer(uint32 idx)
    {
        uint32 value;

        if (UARTCOMMS_ONE_BYTE_WIDTH == UARTCOMMS_txDataBits)
        {
            value = (uint32) UARTCOMMS_txBuffer[idx];
        }
        else
        {
            value  = (uint32) UARTCOMMS_txBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32) UARTCOMMS_txBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }

#endif /* (UARTCOMMS_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
