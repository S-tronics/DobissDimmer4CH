/**********************************************************************************************************************/
/**
 * @file        Std24LC512T.h
 *
 * @author      Stijn Vermeersch
 * @date        29.07.2020
 *
 * @brief       
 *
 *
 * \n<hr>\n
 * Copyright (c) 2021, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#ifndef STD24LC512T_H
#define	STD24LC512T_H
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S Y M B O L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Initialiser for the eeprom 24LC512
 *
 * Initialiser to communicate with the eeprom 24LC512.\n
 */
void Std24LC512TInit(void);

/**
 * @brief   Function to clear the eeprom 24LC512
 *
 * Initialiser to communicate with the eeprom 24LC512.\n
 */
void Std24LC512Clear(void);

/**
 * @brief   Function to write a byte to the eeprom 24LC512
 *
 * Function to write a byte to a register address in the eeprom.\n
 * 
 * @param   address:    Register address of the eeprom to write.
 * @param   byte:       Byte to write.
 * 
 */
void Std24LC512TWriteByte(unsigned int address, unsigned char byte);

/**
 * @brief   Function to write a page to the eeprom 24LC512
 *
 * Function to write a page to a register address in the eeprom.\n
 * 
 * @param   address:    Starting register address of the eeprom to write.
 * @param   data:       Pointer to array of data which contains the starting address of the page.
 * @param   length:     Length of the page
 * 
 */
void Std24LC512TWritePage(unsigned int address, char* data, unsigned char length);

/**
 * @brief   Function to read a page from the eeprom 24LC512
 *
 * Function to read a page from a register address in the eeprom.\n
 * 
 * @param   address:    Starting register address of the eeprom to read from.
 * @param   data:       Pointer to array of data which contains the starting address of the page.
 * @param   length:     Length of the page
 * 
 */
void Std24LC512TReadPage(unsigned int address, char* data, unsigned char length);

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/


#endif	/* STD24LC512T_H */

