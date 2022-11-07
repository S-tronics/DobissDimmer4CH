/**********************************************************************************************************************/
/**
 * @file        gpio\DrvGpio.c
 *
 * @author      Stijn Vermeersch
 * @date        02.01.2017
 *
 * @brief       Basic GPIO manipulation functionality
 *
 * This GPIO manipulating module is designed to be used for initialisation and modifying IO ports and pins.\n
 * The implementation is <em>NOT</em> designed to be FAST!\n
 *
 *
 * \n<hr>\n
 * Copyright (c) 2016, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#define GPIO__DRVGPIO_C
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
#include "DrvGpio.h"
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

//HOOK_GPIO_INCOMING					incoming_gpio_hook;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
unsigned char gpiohandler = 0x00;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvGpioInit(void)
{
    
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioInitPin(GPIO_PORT io_port, unsigned char io_pin_nr, GPIO_PIN_FUNC io_pin_func)
{
	unsigned char io_pin = 0;
    unsigned char* io_ansel_ptr;
    
    switch(io_port)
	{
        case PORT_A:
            io_ansel_ptr = (unsigned char*) &ANCON0;
            switch(io_pin_nr)
            {
                case 0:
                    *io_ansel_ptr &= ~0x01;
                    break;
                case 1:
                    *io_ansel_ptr &= ~0x02;
                    break;   
                case 2:
                    *io_ansel_ptr &= ~0x04;
                    break;  
                case 3:
                    *io_ansel_ptr &= ~0x08;
                    break;
                case 5:
                    *io_ansel_ptr &= ~0x10;
                    break;
                default:
                    break;
            }
            break;
        case PORT_B:
            io_ansel_ptr = (unsigned char*) &ANCON1;
            switch(io_pin_nr)
            {
                case 0:
                    *io_ansel_ptr &= ~0x04;
                    break;
                case 1:
                    *io_ansel_ptr &= ~0x01;
                    break;
                case 4:
                    *io_ansel_ptr &= ~0x02;
                    break;
                default:
                    break;
            }
            break;
        case PORT_C:
            break;
        case PORT_D:
            break;
        case PORT_E:
            io_ansel_ptr = (unsigned char*) &ANCON0;
            switch(io_pin_nr)
            {
                case 0:
                    *io_ansel_ptr &= ~0x20;
                    break;
                case 1:
                    *io_ansel_ptr &= ~0x40;
                    break;
                case 2:
                    *io_ansel_ptr &= ~0x80;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
	io_pin |= (0x01 << io_pin_nr);
	
	DrvGpioInitPinMask(io_port, io_pin, io_pin_func);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioSetPin(GPIO_PORT io_port, unsigned char io_pin_nr)
{
unsigned char addr_port = 0x00;

    DrvGpioSetPinMask(io_port, ((unsigned char)(0x01) << io_pin_nr));
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioClearPin(GPIO_PORT io_port, unsigned char io_pin_nr)
{
unsigned char addr_port = 0x00;

    DrvGpioClearPinMask(io_port, ((unsigned char)(0x01) << io_pin_nr));
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioTogglePin(GPIO_PORT io_port, unsigned char io_pin_nr)
{
unsigned char* io_pin_ptr;
unsigned char port = 0x00;

	switch(io_port)
		{
		case PORT_A:
			io_pin_ptr = (unsigned char*) &LATA;
			break;
		case PORT_B:
			io_pin_ptr = (unsigned char*) &LATB;
			break;
		case PORT_C:
			io_pin_ptr = (unsigned char*) &LATC;
			break;
        case PORT_D:
            io_pin_ptr = (unsigned char*) &LATD;
            break;
        case PORT_E:
            io_pin_ptr = (unsigned char*) &LATE;
            break;
		default:
			break;
		}

	port = *io_pin_ptr;
	port &=	(0x01 << io_pin_nr);

	if(port > 0)
	{
		DrvGpioClearPinMask(io_port, ((unsigned char)(0x01) << io_pin_nr));
	}
	else
	{
		DrvGpioSetPinMask(io_port, ((unsigned char)(0x01) << io_pin_nr));
	}
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool DrvGpioIsPinHigh(GPIO_PORT io_port, unsigned char io_pin_nr)
{
unsigned char* io_pin_ptr;
unsigned char port = 0x00;

	switch(io_port)
		{
		case PORT_A:
			io_pin_ptr = (unsigned char*) &PORTA;
			break;
		case PORT_B:
			io_pin_ptr = (unsigned char*) &PORTB;
			break;
		case PORT_C:
			io_pin_ptr = (unsigned char*) &PORTC;
			break;
        case PORT_D:
            io_pin_ptr = (unsigned char*) &PORTD;
            break;
        case PORT_E:
            io_pin_ptr = (unsigned char*) &PORTE;
            break;
		default:
			break;
		}

	port = *io_pin_ptr;
	if(port & (0x01 << io_pin_nr))
	{
		return true;
	}
    return false;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioInitPinMask(GPIO_PORT io_port, unsigned char io_pin_mask, GPIO_PIN_FUNC io_pin_func)
{
	unsigned char* io_dir_ptr;
    unsigned char* io_set_ptr;
    unsigned char* io_clr_ptr;
    unsigned char* io_ansel_ptr;

	switch(io_port)
	{
	case PORT_A:
		io_dir_ptr = (unsigned char*) &TRISA;
		io_set_ptr = (unsigned char*) &LATA;
		io_clr_ptr = (unsigned char*) &LATA;
        io_ansel_ptr = (unsigned char*) &ANCON0;
		break;
	case PORT_B:
		io_dir_ptr = (unsigned char*) &TRISB;
		io_set_ptr = (unsigned char*) &LATB;
		io_clr_ptr = (unsigned char*) &LATB;
		break;
	case PORT_C:
		io_dir_ptr = (unsigned char*) &TRISC;
		io_set_ptr = (unsigned char*) &LATC;
		io_clr_ptr = (unsigned char*) &LATC;
		break;
    case PORT_D: 
        io_dir_ptr = (unsigned char*) &TRISD;
		io_set_ptr = (unsigned char*) &LATD;
		io_clr_ptr = (unsigned char*) &LATD;
        break;
    case PORT_E:
        io_dir_ptr = (unsigned char*) &TRISE;
		io_set_ptr = (unsigned char*) &LATE;
		io_clr_ptr = (unsigned char*) &LATE;
        break;
	default:
		break;
	}

	if(io_pin_func == PIN_IO_FUNC_INPUT)
	{
		*io_dir_ptr |= io_pin_mask;
        *io_ansel_ptr &= ~io_pin_mask;
	}
	else
	{
		*io_dir_ptr &= ~io_pin_mask;
		if(io_pin_func == PIN_IO_FUNC_OUTPUT_LOW)
		{
			*io_clr_ptr &= ~io_pin_mask;
		}
		else
		{
			if(io_pin_func == PIN_IO_FUNC_OUTPUT_HIGH)
			{
				*io_set_ptr |= io_pin_mask;
			}
		}
	}	
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioSetPinMask(GPIO_PORT io_port, unsigned char io_pin_mask)
{
    unsigned char* io_set_ptr;

	switch(io_port)
	{
	case PORT_A:
		io_set_ptr = (unsigned char*) &LATA;
		break;
	case PORT_B:
		io_set_ptr = (unsigned char*) &LATB;
		break;
	case PORT_C:
		io_set_ptr = (unsigned char*) &LATC;
		break;
    case PORT_D:
        io_set_ptr = (unsigned char*) &LATD;
        break;
    case PORT_E:
        io_set_ptr = (unsigned char*) &LATE;
        break;
	default:
		break;
	}

	*io_set_ptr |= io_pin_mask;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioClearPinMask(GPIO_PORT io_port, unsigned char io_pin_mask)
{
	unsigned char* io_clr_ptr;

	switch(io_port)
	{
	case PORT_A:
		io_clr_ptr = (unsigned char*) &LATA;
		break;
	case PORT_B:
		io_clr_ptr = (unsigned char*) &LATB;
		break;
	case PORT_C:
		io_clr_ptr = (unsigned char*) &LATC;
		break;
    case PORT_D:
        io_clr_ptr = (unsigned char*) &LATD;
        break;
    case PORT_E:
        io_clr_ptr = (unsigned char*) &LATE;
        break;
	default:
		break;
	}
	
	*io_clr_ptr &= ~io_pin_mask;
}

/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char DrvGpioReadPortData(GPIO_PORT io_port)
{
unsigned char* io_pin_ptr;

	switch(io_port)
		{
		case PORT_A:
			io_pin_ptr = (unsigned char*) &PORTA;
			break;
		case PORT_B:
			io_pin_ptr = (unsigned char*) &PORTB;
			break;
		case PORT_C:
			io_pin_ptr = (unsigned char*) &PORTC;
			break;
        case PORT_D:
            io_pin_ptr = (unsigned char*) &PORTD;
            break;
        case PORT_E:
            io_pin_ptr = (unsigned char*) &PORTE;
            break;
		default:
			break;
		}
		
	return (unsigned char)(*io_pin_ptr);
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioIntEnable(GPIO_PORT port, unsigned char io_pin_nr)
{		
		switch(port)
		{
			case PORT_A:
			 	break;
			case PORT_B:
				break;
			case PORT_C:
				break;
            case PORT_D:
                break;
            case PORT_E:
                break;
			default:
			 	break;
		}
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioIntDisable(GPIO_PORT port, unsigned char io_pin_nr)
{
	switch(port)
		{
			case PORT_A:
				
			 	break;
			case PORT_B:
				
				break;
			case PORT_C:
				
				break;
            case PORT_D:
                break;
			default:
			 	break;
		}
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioIncomingIsr(void)				   
{

   	

}
/*--------------------------------------------------------------------------------------------------------------------*/
//void DrvGpioRegisterIncomingIsr(GPIO_PORT port, unsigned char io_pin_nr, SYS_INT_GPIO_MODE mode, HOOK_GPIO_INCOMING hook_gpio_income)
//{
//	incoming_gpio_hook = hook_gpio_income;
//	EXTWAKE &= 0xF0;  				//No External interrupt wakeup for none of te EINTx
//	 
//	switch(port)
//		{
//			case PORT_A:
//				
//			 	break;
//			case PORT_B:
//				
//				break;
//			case PORT_C:
//				
//				break;
//			default:
//			 	break;
//		}
//
//}
/**********************************************************************************************************************/

