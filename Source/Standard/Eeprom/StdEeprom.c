/**********************************************************************************************************************/
/**
 * @file        timer\StdEeprom.c
 *
 * @author      Stijn Vermeersch
 * @date        22.12.2010
 *
 * @brief       .
 *
 *
 *
 * \n<hr>
 * Copyright (c) 2021, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief  Defines the max number of tasks one wants to support
 */
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
//#include <stdio.h>
#include "..\..\System\PIC18F66K80\SysLibAll.h"
#include "..\..\Driver\PIC18F66K80\DrvEeprom.h"

#include "StdEeprom.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void StdEepromInit(void)
{
	
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdEepromRwArray(unsigned int address, STD_EE_ACTION rw, unsigned int* data, unsigned char depth)
{
	unsigned char i = 0;
	
	switch (rw)
	{
		case READ:
			for(i = 0; i < depth; i++)
			{
				data[i] = DrvEepromRead(address);
                address += 2;
			}
			break;
		case WRITE:
			for(i=0; i < depth; i++)
			{
				DrvEepromWrite(address, data[i]);
                address += 2; 
			}
			break;
		default:
			break;
	}
}
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned int StdEepromRead(unsigned int address)
{
    return DrvEepromRead(address);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdEepromWrite(unsigned int address, unsigned int data)
{
    DrvEepromWrite(address, data);
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/
