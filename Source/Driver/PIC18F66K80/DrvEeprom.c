/**********************************************************************************************************************/
/**
 * @file        eeprom\DrvEeprom.c
 *
 * @author      Stijn Vermeersch
 * @date        03.05.2017
 *
 * @brief       Basic Eeprom Functionality
 *
 *
 * \n<hr>\n
 * Copyright (c) 2017, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "..\..\System\PIC18F66K80\SysLibAll.h"

//DRIVER lib include section
#include "DrvEeprom.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define		EEPROM_WR	0x02
#define     EEPROM_RD   0x01
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
typedef enum
{
		READ = 0,
		WRITE = 1
}
EEPROM_ACTION;

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static unsigned char 	eepromintflag = 0x01;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvEepromInitPeripheral(EEPROM_ACTION act, unsigned char addr_l, unsigned char addr_h)
{
	EECON1 &= 0x3F;				//Access data EEPROM memory; Access EEPROM memory
	
	EEADR = addr_l;
	EEADRH = addr_h & 0x03;
	
	switch(act)
	{
		case READ:
			EECON1 &= 0xFB;		//Write cycle inhibit to data EEPROM
			EECON1 |= 0x01;		//Initiate an EEPROM read
			break;
		case WRITE:
            EECON1 &= 0x3F;
			EECON1 |= 0x04;		//Allowing write cycles to EEPROM (WREN)
			break;
		default:
			break;
	}
}
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvEepromInit(void)
{
		
}

/*--------------------------------------------------------------------------------------------------------------------*/
unsigned int DrvEepromRead(unsigned int address)
{
    unsigned int data = 0x0000;
	
	DrvEepromInitPeripheral(READ, (unsigned char)(address & 0x00FF), (unsigned char)((address >> 8) & 0x03));
	address++;
	data |= EEDATA;
	DrvEepromInitPeripheral(READ, (unsigned char)(address & 0x00FF), (unsigned char)((address >> 8) & 0x03));
	data |= (((unsigned int)(EEDATA)) << 8) & 0xFF00;
    
	return data;
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvEepromWrite(unsigned int address, unsigned int wdata)
{
	DrvEepromInitPeripheral(WRITE, (unsigned char)(address & 0x00FF), (unsigned char)((address >> 8) & 0x03));
	
    EEDATA = (unsigned char)(wdata & 0x00FF);
	SysIntDisableAll();
	//Preparing write sequence
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1 |= EEPROM_WR;
	SysIntEnableAll();
	
	while(eepromintflag)
	{
	}
	eepromintflag = 0x01;
	
	address++;
    DrvEepromInitPeripheral(WRITE, (unsigned char)(address & 0x00FF), (unsigned char)((address >> 8) & 0x03));
	EEDATA = (unsigned char)(wdata >> 8);
	SysIntDisableAll();
	//Preparing write sequence
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1 |= EEPROM_WR;
	SysIntEnableAll();
	while(eepromintflag)
	{
	}
	eepromintflag = 0x01;
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvEepromInt(void)
{
	eepromintflag = 0x00;
}

/**********************************************************************************************************************/
