/**********************************************************************************************************************/
/**
 * @file        Appcan.h
 *
 * @author      Stijn Vermeersch
 * @date        27.04.2022
 *
 * @brief      	Application CAN for Dimmer
 *
 *
 *
 * \n<hr>\n
 * Copyright (c) 2022, S-tronics BV\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#ifndef APPCAN_H
#define	APPCAN_H
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include <stdbool.h>
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S Y M B O L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define APPCAN_SUF_ACTION		0x02			//Action
#define APPCAN_SUF_STARTSCAN	0x04			//Start scan / Write config
#define APPCAN_SUF_STOPSCAN		0x05			//Stop scan
#define	APPCAN_SUF_VISUALDET	0x0A			//Visual detection
#define APPCAN_SUF_READDATA		0x10			//Read data
#define APPCAN_SUF_WRITEDATA	0x11			//Write data
#define APPCAN_SUF_STRTBUTSCAN	0x20			//Start Button scan
#define APPCAN_SUF_STPBUTSCAN	0x21			//Stop Button scan
#define APPCAN_SUF_BOOTMODE 	0xF1			//Set in Bootmode
#define APPCAN_SUF_RESET		0xF2			//Reset module
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/

typedef struct
{
    unsigned char gpr;
    unsigned char info;
    unsigned char suffix;
    unsigned char type;
    unsigned char address;
    char* data;
    unsigned char length;
}APPCAN_MESSAGE;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Function AppCan Init\n
 *
 * Initialiser for CAN-bus on application level \n
 * 
 */
void AppCanInit(void);

void AppCanSendStatusUpdate(unsigned char output, unsigned char value);

void AppCanSend(APPCAN_MESSAGE* appmsg);

bool AppCanRead(APPCAN_MESSAGE* appmsg);

/**
 * @brief   Function AppCan Handler\n
 *
 * Handler to execute en control CAN-communication in background. \n
 * 
 */
void AppCanHandler(void);

/**
 * @brief   Functie die eeprom data op de CAN-bus zet\n
 *
 * Deze module zet de eerste 16 bytes van dim op de CAN-bus. \n
 * 
 */
void AppCaneeReadConfigToCan(void);

bool AppCanReceiveNext(unsigned char suffix, APPCAN_MESSAGE* msg);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/
#endif	/* APPCAN_H */
