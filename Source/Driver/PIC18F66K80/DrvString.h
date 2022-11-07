/**********************************************************************************************************************/
/**
 * @file        DrvString.h
 *
 * @author      Stijn Vermeersch
 * @date        30.08.2017
 *
 * @brief       The header file for the Serial Communication Interface
 *
 *
 * \n<hr>
 * Copyright (c) 2017-2018, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#ifndef DRV__STRING_H
#define DRV__STRING_H
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S Y M B O L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   INVALID SPI-device HANDLE definition
 */

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
 * @brief   Calculates the length of a zero-terminated string allocated in DATA space
 *
 * @param   str_ptr :   the string you want to know the length of
 *
 * @return  the calculated length
 */
unsigned char DrvStrGetLength(char* str_ptr);
/**
 * @brief   Fetches the character stored on the address specified by \p str pointer
 *
 * @param   str :       specifies the address to read the char from
 *
 * @return  the requested character value
 */
unsigned char DrvStrConstGetChar(const STRING str);

/**
 * @brief   Checks wether the 2 STRING parameters are equal
 *
 * @param   str1_ptr :  the first STRING parameter
 * @param   str2 :      the second (const) STRING parameter
 *
 * @return  TRUE if both strings are identical
 */
bool DrvStrEquals(char* str1_ptr, const STRING str2);

/**
 * @brief   Checks wether \p str1 starts with \p str2
 *
 * @param   str1_ptr :  the first STRING parameter
 * @param   str2 :      the second (const) STRING parameter
 *
 * @return  TRUE if string 1 starts with string 2
 */
bool DrvStrStartsWith(char* str1_ptr, const STRING str2);

/**
 * @brief   Convert a Char to Int
 *
 * @param   hex_char :               The char that want to be converted
 *
 * @return  The converted char
 */
unsigned char DrvStrCharToInt(unsigned char hex_char);

/**
 * @brief   Decodes a number in string format into SIGNED_32 value
 *
 * If it starts with '0x' the string is interpreted as a hex value, otherwise normal decimal decoding is used.
 * The decoding goes on until an unexpected character is detected.
 * When a negative value is given with the string, the first location is '-'. The negative value has to be
 * decimal.
 *
 *
 * @param   str_ptr :               String which holds the value.
 * @param   length_ptr :             Pointer where you want the character count of the string value to be filled in.
 *
 * @return  The decoded value
 */
signed long DrvStrDecodeValue(char* str_ptr, unsigned char* length_ptr);


/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/

#endif /* DRV__STRING_H */