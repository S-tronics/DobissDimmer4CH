/**********************************************************************************************************************/
/**
 * @file        ccp\DrvCCP.h
 *
 * @author      Stijn Vermeersch
 * @date        02.04.2021
 *
 * @brief       Basic CCP (Capture Compare) register manipulation
 *
 * The CCP-module is used together with a timer to perform Capture Compare functionality.
 *
 *
 * \n<hr>\n
 * Copyright (c) 2021, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#ifndef DRV__CCP_H
#define DRV__CCP_H
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/

//DRIVER lib include section
#include "DrvGpio.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S Y M B O L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   CCP pin name
 */
typedef enum
{
    CH_CCP1 = 0x00,
    CH_CCP2 = 0x01,
	CH_CCP3 = 0x02,
	CH_CCP4 = 0x03,
	CH_CCP5 = 0x04
}
CCP_PIN;

typedef enum
{
    CCP_ACTIVE_H,
    CCP_ACTIVE_L
}
CCP_POL;

typedef struct
{
    unsigned char       chnr;
	CCP_PIN             pin;
    CCP_POL             pol;
    GPIO_PORT           port;
    unsigned char       pinnr;
	unsigned int		period;
	unsigned char		state;	
}
CCP_CHANNEL;

typedef unsigned char	DRVCCP_CH_HNDL;

/**
 * @brief   Prototype of a ADC Conversion done routine
 */

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Initialiser for the CCP-module
 *
 * Registers the CCP module.\n
 * This method must be called only once, which is normally done in the startup phase of the processor.\n
 */
void DrvCCPInit(void);

/**
 * @brief   Function to initialise an CCP channel.
 *
 * Sets processorpin into the correct configuration. Selects the correct timer.\n
 *
 * @param   chnr            : number of channel connected with CCP-pin
 * @param   pin_name        : the CCP pin
 * 
 * @return  DRVCCP_CH_HNDL  : returns a handler for future use as an index that points to the chosen channel.
 * 
 */
DRVCCP_CH_HNDL DrvCCPRegisterChannel(unsigned char chnr, CCP_PIN pin_name);

/**
 * @brief   Function to update a CCP Channel
 *
 * @param   hndl        : Handler of the CCP-channel
 * @param	period      : Period of the CCP-channel (unit = 1000 steps or 1/10 of  1%)
 * 
 */
void DrvCCPUpdate(DRVCCP_CH_HNDL hndl, unsigned int period);

/**
 * @brief   Function to update a CCP Channel
 *
 * @param   hndl        : Handler of the CCP-channel
 * @param   pol         : polarity of the CCP-output
 * 
 */
void DrvCCPStart(DRVCCP_CH_HNDL hndl, CCP_POL pol);

/**
 * @brief   Function to update a CCP Channel
 *
 * @param   hndl        : Handler of the CCP-channel
 * 
 */
void DrvCCPStartTimer(DRVCCP_CH_HNDL hndl);

/**
 * @brief   Function to update a CCP Channel
 *
 * @param   hndl        : Handler of the CCP-channel
 * 
 */
void DrvCCPStop(DRVCCP_CH_HNDL hndl);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/

#endif /* DRV__CCP_H */


