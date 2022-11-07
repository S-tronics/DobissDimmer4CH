/**********************************************************************************************************************/
/**
 * @file        AppLed.h
 *
 * @author      Stijn Vermeersch
 * @date        26.07.2021
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
#ifndef APPLED_H
#define	APPLED_H
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include <stdbool.h>
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
    CH1 = 0,
    CH2 = 1,
    CH3 = 2,
    CH4 = 3,
    POWER = 4,
    BUZZY = 5
}
APPLED_TYPE;

typedef enum
{
    GREEN,
    RED
}
APPLED_COLOR;

typedef struct
{
    APPLED_TYPE type;
    APPLED_COLOR color;
    bool onoff;
}APPLED;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Function to initialise the led control\n
 *
 * Description \n
 * 
 */
void AppLEDInit(void);

/**
 * @brief   Function to control led functionality.
 *
 * Description \n
 * 
 * @param   type:       ledtype:  CH1, CH2, CH3, CH4, POWER, BUZZY
 * @param   onoff:      true = activate the led, false = deactivate the led
 * @param   color:      if the ledtype is a multicolor led, the selected color.
 * 
 */
void AppLEDControl(APPLED_TYPE type, bool onoff, APPLED_COLOR color);

/**
 * @brief   Backgroundhandler to control the leds
 *
 * Place this function in the main backgroundhandler. \n
 * 
 */
void AppLEDHandler(void);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/
#endif	/* APPLED_H */
