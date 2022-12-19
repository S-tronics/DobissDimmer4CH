/**********************************************************************************************************************/
/**
 * @file        AppZCD.h
 *
 * @author      Stijn Vermeersch
 * @date        19.04.2021
 *
 * @brief      
 *
 *
 *
 * \n<hr>\n
 * Copyright (c) 2021, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#ifndef APPZCD_H
#define	APPZCD_H
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "..\Source\Driver\PIC18F66K80\DrvGpio.h"

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S Y M B O L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/


typedef enum
{
    LEADING = 0,
    TRAILING = 1
}PH_CTRL;

//typedef struct
//{
//    unsigned char   ch;
//    bool            en;
//    PH_CTRL         ph;
//    unsigned int    value;
//    GPIO_PORT       port_reset;
//    unsigned char   pin_reset;
//    GPIO_PORT       port_overtemperature;
//    unsigned char   pin_overtemperature;
//    GPIO_PORT       port_overcurrent;
//    unsigned char   pin_overcurrent;
//    bool            overtemperature;
//    bool            overcurrent;
//}
//DIM_CHANNEL;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Initialiser for the functionality of Dimmerchannels
 *
 * Initialiser for the functionality of ZCD (Zero Cross Detection) and \n
 * phase control.
 * 
 */
void AppZCDInit(void);

/**
 * @brief   The function is used to configure a dimmerchannel.
 *
 * Configures a dimmerchannel. 
 * 
 * @param   index:  channel index (Channel 1 = index 0, ...)
 * @param   en:     enable the corresponding channel
 * 
 */
void AppZCDChannelEnable(unsigned char index, bool en);

/**
 * @brief   The function is used to configure a dimmerchannel.
 *
 * Configures a dimmerchannel. 
 * 
 * @param   index:  channel index (Channel 1 = index 0, ...)
 * @param   ph:     phasecontrol can be LEADING or TRAILING edge
 * @param   value:  number of steps (max 1000) or a percentage in 1/10 of a percent.
 * 
 */
void AppZCDConfig(unsigned char index, unsigned char ph, unsigned int value);

/**
 * @brief   This function returns the configuration state.
 *
 * Description\n
 * 
 * @return  bool:    Configuration state.
 */
bool AppZCDGetConfigurated(void);

/**
 * @brief   Handler to do some calculations in background.
 *
 * Handler to do some calculations. This function should be added \n
 * in the main backgroundloop.
 * 
 */
void AppZCDHandler(void);

/**
 * @brief   Function called every time a ZCD occurred.
 * 
 * This function controls the phasecontrol of each Dimmerchannel.
 * 
 */
void AppZCDFlywheel(void);

/**
 * @brief   Function called every time there is a possibility a ZCD occured.
 * 
 * This function controls the phasecontrol of each Dimmerchannel.
 * 
 */
void AppZCDTimer(void);

/**
 * @brief   Function that executes everything after a CCP is done.
 *
 * Function that controls the Dimmerchannels after a CCP is done.
 * 
 * @param   ch:     Dimmerchannel to control.
 * 
 */
void AppZCDChannel(unsigned char ch);

/**
 * @brief   Function that resets a dimmer channel
 *
 * The function reset overcurrentdetection and overtemperature
 * 
 * @param   ch:     Dimmerchannel to reset.
 * 
 */
void AppZCDReset(unsigned char ch);

/**
 * @brief   Function that reads the overcurrent state of a channel
 *
 * This function reads the overcurrent state
 * 
 * @param   ch:     Dimmerchannel to read.
 * 
 * @return  bool:   Overcurrent state 
 */
bool AppZCDGetOverCurrent(unsigned char ch);

/**
 * @brief   Function that reads the overtemperature state of a channel
 *
 * This function reads the overtemperature state
 * 
 * @param   ch:     Dimmerchannel to read.
 * 
 * @return  bool:   Overtemperature state 
 */
bool AppZCDGetOverTemperature(unsigned char ch);


/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/
#endif	/* APPZCD_H */
