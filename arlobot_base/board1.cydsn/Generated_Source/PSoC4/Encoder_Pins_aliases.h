/*******************************************************************************
* File Name: Encoder_Pins.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Encoder_Pins_ALIASES_H) /* Pins Encoder_Pins_ALIASES_H */
#define CY_PINS_Encoder_Pins_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Encoder_Pins_0			(Encoder_Pins__0__PC)
#define Encoder_Pins_0_PS		(Encoder_Pins__0__PS)
#define Encoder_Pins_0_PC		(Encoder_Pins__0__PC)
#define Encoder_Pins_0_DR		(Encoder_Pins__0__DR)
#define Encoder_Pins_0_SHIFT	(Encoder_Pins__0__SHIFT)
#define Encoder_Pins_0_INTR	((uint16)((uint16)0x0003u << (Encoder_Pins__0__SHIFT*2u)))

#define Encoder_Pins_1			(Encoder_Pins__1__PC)
#define Encoder_Pins_1_PS		(Encoder_Pins__1__PS)
#define Encoder_Pins_1_PC		(Encoder_Pins__1__PC)
#define Encoder_Pins_1_DR		(Encoder_Pins__1__DR)
#define Encoder_Pins_1_SHIFT	(Encoder_Pins__1__SHIFT)
#define Encoder_Pins_1_INTR	((uint16)((uint16)0x0003u << (Encoder_Pins__1__SHIFT*2u)))

#define Encoder_Pins_INTR_ALL	 ((uint16)(Encoder_Pins_0_INTR| Encoder_Pins_1_INTR))
#define Encoder_Pins_A			(Encoder_Pins__A__PC)
#define Encoder_Pins_A_PS		(Encoder_Pins__A__PS)
#define Encoder_Pins_A_PC		(Encoder_Pins__A__PC)
#define Encoder_Pins_A_DR		(Encoder_Pins__A__DR)
#define Encoder_Pins_A_SHIFT	(Encoder_Pins__A__SHIFT)
#define Encoder_Pins_A_INTR	((uint16)((uint16)0x0003u << (Encoder_Pins__0__SHIFT*2u)))

#define Encoder_Pins_B			(Encoder_Pins__B__PC)
#define Encoder_Pins_B_PS		(Encoder_Pins__B__PS)
#define Encoder_Pins_B_PC		(Encoder_Pins__B__PC)
#define Encoder_Pins_B_DR		(Encoder_Pins__B__DR)
#define Encoder_Pins_B_SHIFT	(Encoder_Pins__B__SHIFT)
#define Encoder_Pins_B_INTR	((uint16)((uint16)0x0003u << (Encoder_Pins__1__SHIFT*2u)))


#endif /* End Pins Encoder_Pins_ALIASES_H */


/* [] END OF FILE */
