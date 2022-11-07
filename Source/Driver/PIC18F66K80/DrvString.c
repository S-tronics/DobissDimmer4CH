/**********************************************************************************************************************/
/**
 * @file        spi\DrvString.c
 *
 * @author      Stijn Vermeersch
 * @date        30.08.2017
 *
 * @brief       
 *
 * \n<hr>
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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "..\..\System\PIC18F66K80\SysLibAll.h"

//DRIVER lib include section
#include "DrvString.h"
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

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
unsigned char DrvStrGetLength(char* str_ptr)
{
    unsigned char i = 0;
    for(;;)
    {
        if(str_ptr[i] == 0)
        {
            return i;
        }
        i++;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char DrvStrConstGetChar(const STRING str)
{
	return str[0] & 0xFF;
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool DrvStrEquals(char* str1_ptr, const STRING str2)
{
	for(;;)
	{
			if(*str1_ptr == 0)
			{
				if(DrvStrConstGetChar(str2) == 0)
				{
					return true;
				}
			}
			if(*str1_ptr != DrvStrConstGetChar(str2))
			{
				return false;
			}
			str1_ptr++;
			str2++;
	}
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool DrvStrStartsWith(char* str1_ptr, const STRING str2)
{
    for(;;)
    {
        if(DrvStrConstGetChar(str2) == 0)
        {
            return true;
        }
        if(*str1_ptr != DrvStrConstGetChar(str2))
        {
            return false;
        }
        str1_ptr++;
        str2++;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char DrvStrCharToInt(unsigned char hex_char)
{
    if((hex_char >= '0') && (hex_char <= '9'))
    {
        return (hex_char - '0');
    }
    else if((hex_char >= 'a') && (hex_char <= 'f'))
    {
        return (hex_char - 'a' + 10);
    }
    else if((hex_char >= 'A') && (hex_char <= 'F'))
    {
        return (hex_char - 'A' + 10);
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
signed long DrvStrDecodeValue(char* str_ptr, unsigned char* length_ptr)
{
    char* ptr_ptr;
    signed long value  = 0;

    if(DrvStrStartsWith(str_ptr,"0x"))
    {
        ptr_ptr = &str_ptr[2];
        for(;;)
        {
            if((*ptr_ptr >= '0') && (*ptr_ptr <= '9'))
            {
                value <<= 4;
                value += (*ptr_ptr) - '0';
            }
            else if((*ptr_ptr >= 'a') && (*ptr_ptr <= 'f'))
            {
                value <<= 4;
                value += (*ptr_ptr) - 'a' + 10;
            }
            else if((*ptr_ptr >= 'A') && (*ptr_ptr <= 'F'))
            {
                value <<= 4;
                value += (*ptr_ptr) - 'A' + 10;
            }
            else
            {
                if(length_ptr != NULL)
                {
                    *length_ptr = ptr_ptr - &str_ptr[0];
                }
                return value;
            }
            ptr_ptr++;
        }
    }
    else
    {
        if(str_ptr[0] == '-')
        {
            ptr_ptr = &str_ptr[1];
			for(;;)
            {
                if((*ptr_ptr < '0') || (*ptr_ptr > '9'))
                {
                    if(length_ptr != NULL)
                    {
                        *length_ptr = ptr_ptr - &str_ptr[1];
                    }
                    value = -value;
                    return value;
                }
                value *= 10;
                value += (*ptr_ptr) - '0';
                ptr_ptr++;
            }
        }
        else
        {
            ptr_ptr = &str_ptr[0];
			for(;;)
            {
                if((*ptr_ptr < '0') || (*ptr_ptr > '9'))
                {
                    if(length_ptr != NULL)
                    {
                        *length_ptr = ptr_ptr - &str_ptr[0];
                    }
                    return value;
                }
                value *= 10;
                value += (*ptr_ptr) - '0';
                ptr_ptr++;
            }
        }
		for(;;)
        {
            if((*ptr_ptr < '0') || (*ptr_ptr > '9'))
            {
                if(length_ptr != NULL)
                {
                    *length_ptr = ptr_ptr - &str_ptr[0];
                }
                return value;
            }
            value *= 10;
            value += (*ptr_ptr) - '0';
            ptr_ptr++;
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************/
