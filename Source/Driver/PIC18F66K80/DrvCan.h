/**********************************************************************************************************************/
/**
 * @file        can\DrvCan.h
 *
 * @author      Stijn Vermeersch
 * @date        27.07.2021
 *
 * @brief       Basic CAN-controller functionality
 *
 * The CAN-controller is used to maintain CAN-communication
 *
 *
 * \n<hr>\n
 * Copyright (c) 2021, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#ifndef DRV__CAN_H
#define DRV__CAN_H
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
typedef union 
{
    unsigned long id;
    struct 
    {
        unsigned char BYTE1;
        unsigned char BYTE2;
        unsigned char BYTE3;
        unsigned char BYTE4;
    }BYTES;
}
CAN_MESSAGE_ID;

typedef struct
{
    CAN_MESSAGE_ID  id;                         //CAN-msg ID
    unsigned char   dlc;                        //Data length counter
    char   data[8];                    //Data 8 bytes
    char   bufnr;
}
CAN_MESSAGE;

typedef void (*DRVCAN_RXHOOK)(void);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Initialiser for the CAN-module
 *
 * Registers the CAN module.\n
 * This method must be called only once, which is normally done in the startup phase of the processor.\n
 * 
 * @param   address: \n
 * 
 */
void DrvCanInit(unsigned char address);

/**
 * @brief   This function sends a CAN-message
 * 
 */
bool DrvCanRead(CAN_MESSAGE* msg);

void DrvCanSend(CAN_MESSAGE* msg);

/**
 * @brief   This function sends a CAN-message
 * 
 */
void DrvCanRxISR(void);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/

#endif /* DRV__CAN_H */


