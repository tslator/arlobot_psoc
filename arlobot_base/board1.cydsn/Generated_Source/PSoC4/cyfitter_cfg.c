/*******************************************************************************
* File Name: cyfitter_cfg.c
* 
* PSoC Creator  3.3 SP1
*
* Description:
* This file contains device initialization code.
* Except for the user defined sections in CyClockStartupError(), this file should not be modified.
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2015 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <string.h>
#include "cytypes.h"
#include "cydevice_trm.h"
#include "cyfitter.h"
#include "CyLib.h"
#include "CyLFClk.h"
#include "cyfitter_cfg.h"


#if defined(__GNUC__) || defined(__ARMCC_VERSION)
    #define CYPACKED 
    #define CYPACKED_ATTR __attribute__ ((packed))
    #define CYALIGNED __attribute__ ((aligned))
    #define CY_CFG_UNUSED __attribute__ ((unused))
    #ifndef CY_CFG_SECTION
        #define CY_CFG_SECTION __attribute__ ((section(".psocinit")))
    #endif
    
    #if defined(__ARMCC_VERSION)
        #define CY_CFG_MEMORY_BARRIER() __memory_changed()
    #else
        #define CY_CFG_MEMORY_BARRIER() __sync_synchronize()
    #endif
    
#elif defined(__ICCARM__)
    #include <intrinsics.h>

    #define CYPACKED __packed
    #define CYPACKED_ATTR 
    #define CYALIGNED _Pragma("data_alignment=4")
    #define CY_CFG_UNUSED _Pragma("diag_suppress=Pe177")
    #define CY_CFG_SECTION _Pragma("location=\".psocinit\"")
    
    #define CY_CFG_MEMORY_BARRIER() __DMB()
    
#else
    #error Unsupported toolchain
#endif


CY_CFG_UNUSED
static void CYMEMZERO(void *s, size_t n);
CY_CFG_UNUSED
static void CYMEMZERO(void *s, size_t n)
{
	(void)memset(s, 0, n);
}
CY_CFG_UNUSED
static void CYCONFIGCPY(void *dest, const void *src, size_t n);
CY_CFG_UNUSED
static void CYCONFIGCPY(void *dest, const void *src, size_t n)
{
	(void)memcpy(dest, src, n);
}
CY_CFG_UNUSED
static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n);
CY_CFG_UNUSED
static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n)
{
	(void)memcpy(dest, src, n);
}



/* Clock startup error codes                                                   */
#define CYCLOCKSTART_NO_ERROR    0u
#define CYCLOCKSTART_XTAL_ERROR  1u
#define CYCLOCKSTART_32KHZ_ERROR 2u
#define CYCLOCKSTART_PLL_ERROR   3u

#ifdef CY_NEED_CYCLOCKSTARTUPERROR
/*******************************************************************************
* Function Name: CyClockStartupError
********************************************************************************
* Summary:
*  If an error is encountered during clock configuration (crystal startup error,
*  PLL lock error, etc.), the system will end up here.  Unless reimplemented by
*  the customer, this function will stop in an infinite loop.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode);
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode)
{
    /* To remove the compiler warning if errorCode not used.                */
    errorCode = errorCode;

    /* `#START CyClockStartupError` */

    /* If we have a clock startup error (bad MHz crystal, PLL lock, etc.),  */
    /* we will end up here to allow the customer to implement something to  */
    /* deal with the clock condition.                                       */

    /* `#END` */

    /* If nothing else, stop here since the clocks have not started         */
    /* correctly.                                                           */
    while(1) {}
}
#endif

#define CY_CFG_BASE_ADDR_COUNT 7u
CYPACKED typedef struct
{
	uint8 offset;
	uint8 value;
} CYPACKED_ATTR cy_cfg_addrvalue_t;



/*******************************************************************************
* Function Name: cfg_write_bytes32
********************************************************************************
* Summary:
*  This function is used for setting up the chip configuration areas that
*  contain relatively sparse data.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[]);
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[])
{
	/* For 32-bit little-endian architectures */
	uint32 i, j = 0u;
	for (i = 0u; i < CY_CFG_BASE_ADDR_COUNT; i++)
	{
		uint32 baseAddr = addr_table[i];
		uint8 count = (uint8)baseAddr;
		baseAddr &= 0xFFFFFF00u;
		while (count != 0u)
		{
			CY_SET_XTND_REG8((void CYFAR *)(baseAddr + data_table[j].offset), data_table[j].value);
			j++;
			count--;
		}
	}
}


/*******************************************************************************
* Function Name: ClockSetup
********************************************************************************
*
* Summary:
*   Performs the initialization of all of the clocks in the device based on the
*   settings in the Clock tab of the DWR.  This includes enabling the requested
*   clocks and setting the necessary dividers to produce the desired frequency. 
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
static void ClockSetup(void);
CY_CFG_SECTION
static void ClockSetup(void)
{

	/* Set Flash Cycles based on max possible frequency in case a glitch occurs during ClockSetup(). */
	CY_SET_REG32((void CYXDATA *)(CYREG_CPUSS_FLASH_CTL), (0x0012u));
	/* Setup and trim IMO based on desired frequency. */
	CySysClkWriteImoFreq(48u);
	/* CYDEV_CLK_ILO_CONFIG Starting address: CYDEV_CLK_ILO_CONFIG */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_CLK_ILO_CONFIG), 0x80000006u);


	/* Setup phase aligned clocks */
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL4, 0x00002F00u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF44u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL3, 0x00002F00u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF43u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL2, 0x00002F00u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF42u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL1, 0x00002200u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF41u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL0, 0x00001D00u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF40u);

	/* CYDEV_CLK_IMO_CONFIG Starting address: CYDEV_CLK_IMO_CONFIG */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_CLK_IMO_CONFIG), 0x82000000u);

	/* CYDEV_CLK_SELECT Starting address: CYDEV_CLK_SELECT */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_CLK_SELECT), 0x00040000u);

	/* CYDEV_PERI_PCLK_CTL12 Starting address: CYDEV_PERI_PCLK_CTL12 */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_PERI_PCLK_CTL12), 0x00000044u);

	/* CYDEV_PERI_PCLK_CTL11 Starting address: CYDEV_PERI_PCLK_CTL11 */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_PERI_PCLK_CTL11), 0x00000043u);

	/* CYDEV_PERI_PCLK_CTL10 Starting address: CYDEV_PERI_PCLK_CTL10 */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_PERI_PCLK_CTL10), 0x00000042u);

	/* CYDEV_PERI_PCLK_CTL5 Starting address: CYDEV_PERI_PCLK_CTL5 */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_PERI_PCLK_CTL5), 0x00000041u);

	/* CYDEV_PERI_PCLK_CTL2 Starting address: CYDEV_PERI_PCLK_CTL2 */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_PERI_PCLK_CTL2), 0x00000040u);

	(void)CyIntSetVector(7u, &CySysWdtIsr);
	CyIntEnable(7u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_WDT_CONFIG), 0x00000000u);
}


/* Analog API Functions */


/*******************************************************************************
* Function Name: AnalogSetDefault
********************************************************************************
*
* Summary:
*  Sets up the analog portions of the chip to default values based on chip
*  configuration options from the project.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void AnalogSetDefault(void);
static void AnalogSetDefault(void)
{
	CY_SET_XTND_REG32((void CYFAR *)CYREG_CTBM0_DFT_CTRL, 0x00000003u);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_CTBM1_DFT_CTRL, 0x00000003u);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_SAR_CTRL, 0x08000000u);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_SAR_MUX_SWITCH0, 0x0000000Fu);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_SAR_MUX_SWITCH_HW_CTRL, 0x0000000Fu);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_PASS_DSAB_DSAB_CTRL, 0x00000000u);
	/* Variable VDDA is selected; no pumps are enabled by default */
}


/*******************************************************************************
* Function Name: SetAnalogRoutingPumps
********************************************************************************
*
* Summary:
* Enables or disables the analog pumps feeding analog routing switches.
* Intended to be called at startup, based on the Vdda system configuration;
* may be called during operation when the user informs us that the Vdda voltage crossed the pump threshold.
*
* Parameters:
*  enabled - 1 to enable the pumps, 0 to disable the pumps
*
* Return:
*  void
*
*******************************************************************************/
void SetAnalogRoutingPumps(uint8 enabled)
{
	uint32 regValue = CY_GET_XTND_REG32((void *)(CYREG_SAR_PUMP_CTRL));
	if (enabled != 0u)
	{
		regValue |= 0x80000000u;
	}
	else
	{
		regValue &= ~0x80000000u;
	}
	CY_SET_XTND_REG32((void *)(CYREG_SAR_PUMP_CTRL), regValue);
}

#define CY_AMUX_UNUSED CYREG_CM0_ROM_DWT


/*******************************************************************************
* Function Name: cyfitter_cfg
********************************************************************************
* Summary:
*  This function is called by the start-up code for the selected device. It
*  performs all of the necessary device configuration based on the design
*  settings.  This includes settings from the Design Wide Resources (DWR) such
*  as Clocks and Pins as well as any component configuration that is necessary.
*
* Parameters:  
*   void
*
* Return:
*   void
*
*******************************************************************************/
CY_CFG_SECTION
void cyfitter_cfg(void)
{
	/* Disable interrupts by default. Let user enable if/when they want. */
	CyGlobalIntDisable;

	{
		static const uint32 CYCODE cy_cfg_addr_table[] = {
			0x400F3010u, /* Base address: 0x400F3000 Count: 16 */
			0x400F310Fu, /* Base address: 0x400F3100 Count: 15 */
			0x400F3308u, /* Base address: 0x400F3300 Count: 8 */
			0x400F4007u, /* Base address: 0x400F4000 Count: 7 */
			0x400F410Au, /* Base address: 0x400F4100 Count: 10 */
			0x400F430Du, /* Base address: 0x400F4300 Count: 13 */
			0x400F6002u, /* Base address: 0x400F6000 Count: 2 */
		};

		static const cy_cfg_addrvalue_t CYCODE cy_cfg_data_table[] = {
			{0x3Du, 0x40u},
			{0x59u, 0x04u},
			{0x5Fu, 0x01u},
			{0x86u, 0x04u},
			{0x9Au, 0x01u},
			{0x9Eu, 0x02u},
			{0xA4u, 0x09u},
			{0xA6u, 0x06u},
			{0xAAu, 0x08u},
			{0xACu, 0x0Au},
			{0xAEu, 0x05u},
			{0xB6u, 0x0Fu},
			{0xBEu, 0x40u},
			{0xD8u, 0x04u},
			{0xDBu, 0x04u},
			{0xDFu, 0x01u},
			{0x05u, 0x08u},
			{0x0Cu, 0x80u},
			{0x0Du, 0x20u},
			{0x15u, 0x40u},
			{0x17u, 0x24u},
			{0x1Fu, 0x40u},
			{0x20u, 0x01u},
			{0x6Du, 0x40u},
			{0x6Fu, 0x04u},
			{0x78u, 0x10u},
			{0x80u, 0x80u},
			{0xC0u, 0x20u},
			{0xC2u, 0xC0u},
			{0xC4u, 0xE0u},
			{0xDEu, 0x04u},
			{0x88u, 0x10u},
			{0x9Cu, 0x10u},
			{0xA9u, 0x08u},
			{0xABu, 0x20u},
			{0xAFu, 0x40u},
			{0xB4u, 0x01u},
			{0xB5u, 0x20u},
			{0xECu, 0x40u},
			{0x00u, 0x01u},
			{0x02u, 0x01u},
			{0x05u, 0x10u},
			{0x06u, 0x20u},
			{0x82u, 0x01u},
			{0xC0u, 0xF0u},
			{0xE0u, 0x20u},
			{0x4Bu, 0x10u},
			{0x93u, 0x10u},
			{0xA8u, 0x01u},
			{0xA9u, 0x10u},
			{0xAFu, 0x10u},
			{0xB6u, 0x20u},
			{0xD2u, 0x10u},
			{0xE8u, 0x80u},
			{0xEAu, 0x10u},
			{0xECu, 0x20u},
			{0x00u, 0x08u},
			{0x1Cu, 0x20u},
			{0x30u, 0x08u},
			{0x54u, 0x10u},
			{0x88u, 0x04u},
			{0xA0u, 0x08u},
			{0xB4u, 0x08u},
			{0xC0u, 0x08u},
			{0xC6u, 0x04u},
			{0xCCu, 0x02u},
			{0xD4u, 0x02u},
			{0xE2u, 0x04u},
			{0xECu, 0x08u},
			{0x02u, 0x01u},
			{0x11u, 0x01u},
		};



		CYPACKED typedef struct {
			void CYFAR *address;
			uint16 size;
		} CYPACKED_ATTR cfg_memset_t;

		static const cfg_memset_t CYCODE cfg_memset_list [] = {
			/* address, size */
			{(void CYFAR *)(CYDEV_UDB_P0_U0_BASE), 1024u},
			{(void CYFAR *)(CYDEV_UDB_DSI0_BASE), 1024u},
		};

		uint8 CYDATA i;

		/* Zero out critical memory blocks before beginning configuration */
		for (i = 0u; i < (sizeof(cfg_memset_list)/sizeof(cfg_memset_list[0])); i++)
		{
			const cfg_memset_t CYCODE * CYDATA ms = &cfg_memset_list[i];
			CYMEMZERO(ms->address, (size_t)(uint32)(ms->size));
		}

		cfg_write_bytes32(cy_cfg_addr_table, cy_cfg_data_table);

		/* HSIOM Starting address: CYDEV_HSIOM_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_HSIOM_PORT_SEL3), 0x0000EE02u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_HSIOM_PORT_SEL4), 0x000000EEu);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_HSIOM_PORT_SEL7), 0x00000099u);

		/* UDB_PA_0 Starting address: CYDEV_UDB_PA0_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_UDB_PA0_BASE), 0x00990004u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_UDB_PA0_CFG4), 0x00AA0000u);

		/* UDB_PA_1 Starting address: CYDEV_UDB_PA1_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_UDB_PA1_BASE), 0x00990000u);

		/* UDB_PA_2 Starting address: CYDEV_UDB_PA2_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_UDB_PA2_BASE), 0x00990000u);

		/* UDB_PA_3 Starting address: CYDEV_UDB_PA3_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_UDB_PA3_BASE), 0x00990000u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_UDB_PA3_CFG8), 0x00010000u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_UDB_PA3_CFG12), 0x00000002u);

		/* TCPWM_CNT0 Starting address: CYDEV_TCPWM_CNT0_TR_CTRL0 */
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_TCPWM_CNT0_TR_CTRL0), 0x000EEE1Eu);

		/* Enable digital routing */
		CY_SET_XTND_REG8((void *)CYREG_UDB_UDBIF_BANK_CTL, CY_GET_XTND_REG8((void *)CYREG_UDB_UDBIF_BANK_CTL) | 0x02u);

		/* Enable UDB array */
		CY_SET_XTND_REG8((void *)CYREG_UDB_UDBIF_WAIT_CFG, (uint8)((CY_GET_XTND_REG8((void *)CYREG_UDB_UDBIF_WAIT_CFG) & 0xC3u) | 0x14u));
		CY_SET_XTND_REG8((void *)CYREG_UDB_UDBIF_BANK_CTL, (uint8)(CY_GET_XTND_REG8((void *)CYREG_UDB_UDBIF_BANK_CTL) | 0x16u));
	}


	/* Perform second pass device configuration. These items must be configured in specific order after the regular configuration is done. */
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_UDB_P0_U0_CFG24), 0x40u);
	/* IOPINS0_0 Starting address: CYDEV_GPIO_PRT0_BASE */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT0_PC), 0x0000024Eu);

	/* IOPINS0_1 Starting address: CYDEV_GPIO_PRT1_BASE */
	CY_SET_XTND_REG32((void CYFAR *)(CYDEV_GPIO_PRT1_BASE), 0x00000080u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT1_PC2), 0x00000080u);

	/* IOPINS0_2 Starting address: CYDEV_GPIO_PRT2_BASE */
	CY_SET_XTND_REG32((void CYFAR *)(CYDEV_GPIO_PRT2_BASE), 0x0000000Fu);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT2_PC2), 0x0000000Fu);

	/* IOPINS0_3 Starting address: CYDEV_GPIO_PRT3_BASE */
	CY_SET_XTND_REG32((void CYFAR *)(CYDEV_GPIO_PRT3_BASE), 0x00000032u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT3_PC), 0x00012DB3u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT3_INTR_CFG), 0x00000F00u);

	/* IOPINS0_4 Starting address: CYDEV_GPIO_PRT4_BASE */
	CY_SET_XTND_REG32((void CYFAR *)(CYDEV_GPIO_PRT4_BASE), 0x00000003u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT4_PC), 0x00000024u);

	/* IOPINS0_5 Starting address: CYDEV_GPIO_PRT5_BASE */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT5_PC), 0x00000DB6u);

	/* IOPINS0_7 Starting address: CYDEV_GPIO_PRT7_BASE */
	CY_SET_XTND_REG32((void CYFAR *)(CYDEV_GPIO_PRT7_BASE), 0x00000002u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT7_PC), 0x00000031u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT7_PC2), 0x00000002u);


	/* Setup clocks based on selections from Clock DWR */
	ClockSetup();

	/* Perform basic analog initialization to defaults */
	AnalogSetDefault();

}
