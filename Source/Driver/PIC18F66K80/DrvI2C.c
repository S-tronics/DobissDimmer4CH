/**********************************************************************************************************************/
/**
 * @file        i2c\DrvI2C.c
 *
 * @author      Stijn Vermeersch
 * @date        22.12.2010
 *
 * @brief       This file contains the implementation of the master SPI driver routines
 *
 * The SPI master driver can manage multiple SPI slave devices.  These SPI slaves have there own CS pin, baudrate,
 * number of databits and clocking scheme.  All these settings are specified on registering the slave device.
 * A valid SPI-device HANDLE is the result of succesfull registration.  Using this handle, the application can
 * read and write data to the slave device.\n\n
 *
 * A read or write operation must not be interrupted by another read or write operation of another SPI device on the
 * same channel. If this happens anyway, an exception is thrown.
 *
 * Watch out: it is assumed that chip select pins are always pulled up!
 *
 *
 * \n<hr>
 * Copyright (c) 2009, TVH\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#define I2C__DRVI2C_C
/**********************************************************************************************************************/



/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "..\..\..\Source\System\PIC18F66K80\SysLibAll.h"

//DRIVER lib include section
#include "DrvI2C.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define I2C_PORT_COUNT	2           

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
typedef enum
{
    I2C_STRT = 0,
    I2C_RPSTRT = 1,
    I2C_ACK = 2,
    I2C_WRITE = 3,
    I2C_RD = 4,
    I2C_STP = 5,
    I2C_IDLE = 6
}
I2C_STATE;

typedef struct
{
	volatile unsigned char* ssp_stat;
	volatile unsigned char* ssp_ctrl_reg1;
    volatile unsigned char* ssp_ctrl_reg2;
	volatile unsigned char* ssp_ctrl_reg3;
	volatile unsigned char* ssp_mask;
	volatile unsigned char* ssp_add_baudrate_reg;
	volatile unsigned char* ssp_data_buf;
}
I2C_BUS;

typedef struct
{
	I2C_CHANNEL         ch;
	unsigned char		address;
	unsigned long		speed;
	I2C_MSTR_SLV        mode;
}
DRVI2C_DEV_PROPS;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvI2CChannelInit(I2C_CHANNEL ch, I2C_MSTR_SLV mode, unsigned int speed, unsigned char address);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static I2C_BUS 				drvi2cport[I2C_PORT_COUNT];
static DRVI2C_DEV_PROPS		drvi2cdevice[8];

//State flag for statemachine
static I2C_STATE i2cstate = I2C_IDLE;
static BOOLEAN i2cintflag = FALSE;

static DRVI2C_DEV_HNDL		drvi2c_registered_devcount;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvI2CChannelInit(I2C_CHANNEL ch, I2C_MSTR_SLV mode, unsigned int speed, unsigned char address)
{
	I2C_BUS*  i2cbusptr;
	switch(ch)
	{
		case I2C_CHANNEL_A:		//I2C-channel 1
			TRISC |= 0x18;
			i2cbusptr = &drvi2cport[I2C_CHANNEL_A];
			drvi2cport[I2C_CHANNEL_A].ssp_stat = &SSPSTAT;
			drvi2cport[I2C_CHANNEL_A].ssp_ctrl_reg1 = &SSPCON1;
			drvi2cport[I2C_CHANNEL_A].ssp_ctrl_reg2 = &SSPCON2;
			//drvi2cport[I2C_CHANNEL_A].ssp_mask = &SSPMSK;
			drvi2cport[I2C_CHANNEL_A].ssp_add_baudrate_reg = &SSPADD;
			drvi2cport[I2C_CHANNEL_A].ssp_data_buf = &SSPBUF;
			break;
		default:
			break;
	}
	
	*i2cbusptr->ssp_stat &= ~0x40;   	//Slew rate controle disabled + SMBbus specific inputs disabled
    *i2cbusptr->ssp_stat |= 0x80;
	//*i2cbusptr->ssp_ctrl_reg2 |= 0x08;   //Same for master & slave
	//*i2cbusptr->ssp_ctrl_reg3 &= 0x00;   //Same for master & slave
	
	switch(mode)
	{
		case I2C_MASTER:
			*i2cbusptr->ssp_ctrl_reg1 |= 0x28;	//I2c-pins enabled; I2C master
			*i2cbusptr->ssp_add_baudrate_reg = (unsigned char)(((Get_SysClk() / speed) >> 2) - 1);
			break;
		case I2C_SLAVE:
			*i2cbusptr->ssp_ctrl_reg1 |= 0x36;	//I2c-pins enabled; Clock enable; I2C slave mode, 7-bit address
			*i2cbusptr->ssp_mask &= ~0x01;
			*i2cbusptr->ssp_add_baudrate_reg = (unsigned char)(address << 1);
			break;
		default:
			break;
	}

}
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvI2CInit(void)
{
	drvi2c_registered_devcount = 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DRVI2C_DEV_HNDL DrvI2CRegisterDevice(I2C_CHANNEL ch, unsigned char address, unsigned long speed, I2C_MSTR_SLV mode)
{
	drvi2cdevice[drvi2c_registered_devcount].ch = ch;
	drvi2cdevice[drvi2c_registered_devcount].address = address << (unsigned char)(1);
	drvi2cdevice[drvi2c_registered_devcount].speed = speed;
	drvi2cdevice[drvi2c_registered_devcount].mode = mode;
	
	DrvI2CChannelInit(ch, mode, speed, address);
	drvi2c_registered_devcount++;
	
	return (drvi2c_registered_devcount - 1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvI2CConfigDevice(DRVI2C_DEV_HNDL hndl)
{
    DRVI2C_DEV_PROPS* dev_ptr;
	I2C_BUS* bus_ptr;
    
    dev_ptr = &drvi2cdevice[hndl];
	bus_ptr = &drvi2cport[dev_ptr->ch];
    DrvI2CChannelInit(dev_ptr->ch, dev_ptr->mode, dev_ptr->speed, dev_ptr->address);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvI2CStart(DRVI2C_DEV_HNDL hndl)
{
	DRVI2C_DEV_PROPS* dev_ptr;
	I2C_BUS* bus_ptr;
	
	dev_ptr = &drvi2cdevice[hndl];
	bus_ptr = &drvi2cport[dev_ptr->ch];

    while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
    {
        asm("nop");//Timeout should be added here.
    }
	*bus_ptr->ssp_ctrl_reg2 |= 0x01;			//Set SEN

}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvI2CRepeatedStart(DRVI2C_DEV_HNDL hndl)
{
	DRVI2C_DEV_PROPS* dev_ptr;
	I2C_BUS* bus_ptr;
	
	dev_ptr = &drvi2cdevice[hndl];
	bus_ptr = &drvi2cport[dev_ptr->ch];

    while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
    {
        asm("nop");//Timeout should be added here.
    }
	*bus_ptr->ssp_ctrl_reg2 |= 0x02;

}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvI2CStop(DRVI2C_DEV_HNDL hndl)
{
	DRVI2C_DEV_PROPS* dev_ptr;
	I2C_BUS* bus_ptr;
    unsigned char temp = 0x00;
	
	dev_ptr = &drvi2cdevice[hndl];
	bus_ptr = &drvi2cport[dev_ptr->ch]; 
    
	while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
    {
        asm("nop");//Timeout should be added here.
    }
	*bus_ptr->ssp_ctrl_reg2 |= 0x04;
    while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
    {
        asm("nop");//Timeout should be added here.
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvI2CWriteMstr(DRVI2C_DEV_HNDL hndl, char* data)
{
	DRVI2C_DEV_PROPS* dev_ptr;
	I2C_BUS* bus_ptr;
	
	dev_ptr = &drvi2cdevice[hndl];
	bus_ptr = &drvi2cport[dev_ptr->ch];
	
	while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
	{
	}
	*bus_ptr->ssp_data_buf = *data;
	while((*bus_ptr->ssp_stat & 0x01) == 0x01)      //Waiting for clearing BF-bit
	{
	}
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvI2CWriteMstrArr(DRVI2C_DEV_HNDL hndl, char* data, unsigned char length)
{
    DRVI2C_DEV_PROPS* dev_ptr;
	I2C_BUS* bus_ptr;
	
	dev_ptr = &drvi2cdevice[hndl];
	bus_ptr = &drvi2cport[dev_ptr->ch];
	while(length != 0)
	{
		while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
		{
		}
		*bus_ptr->ssp_data_buf = *data;
		while((*bus_ptr->ssp_stat & 0x01) == 0x01)      //Waiting for clearing BF-bit
		{
		
		}
		data++;
		length--;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvI2CWriteAddrMstr(DRVI2C_DEV_HNDL hndl, I2C_RW rw)
{
	DRVI2C_DEV_PROPS* dev_ptr;
	I2C_BUS* bus_ptr;
	
	dev_ptr = &drvi2cdevice[hndl];
	bus_ptr = &drvi2cport[dev_ptr->ch];
    while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
    {
        asm("nop");//Timeout should be added here.
    }
	if(rw == I2C_R)
    {
        *bus_ptr->ssp_data_buf = dev_ptr->address | 0x01;
    }
	else if(rw == I2C_W) //RW-bit is cleared when writing
    {
        *bus_ptr->ssp_data_buf = dev_ptr->address & 0xFE;
    } 
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvI2CReadMstr(DRVI2C_DEV_HNDL hndl, char* data, BOOLEAN last)
{
	DRVI2C_DEV_PROPS* dev_ptr;
	I2C_BUS* bus_ptr;
	
	dev_ptr = &drvi2cdevice[hndl];
	bus_ptr = &drvi2cport[dev_ptr->ch];
	//Read data
    while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
    {
    }
	*bus_ptr->ssp_ctrl_reg2 |= 0x08;			//Set receive enable bit
	
    //Set Acknowledge bit
    while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
    {
    }
    *data = *bus_ptr->ssp_data_buf;
    
    if(last == FALSE)
    {
        *bus_ptr->ssp_ctrl_reg2 &= ~0x20;
    }
    else
    {
        *bus_ptr->ssp_ctrl_reg2 |= 0x20;
    }
    *bus_ptr->ssp_ctrl_reg2 |= 0x10;
    
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvI2CReadMstrArr(DRVI2C_DEV_HNDL hndl, char* data, unsigned char length)
{
    DRVI2C_DEV_PROPS* dev_ptr;
	I2C_BUS* bus_ptr;
	
	dev_ptr = &drvi2cdevice[hndl];
	bus_ptr = &drvi2cport[dev_ptr->ch];
    while(length != 0)
    {
        while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
        {
        }
        *bus_ptr->ssp_ctrl_reg2 |= 0x08;			//Set receive enable bit
        //Set Acknowledge bit
        while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
        {
        }
        *data = *bus_ptr->ssp_data_buf;
        data++;
        length--;
        if(length != 0)
        {
            *bus_ptr->ssp_ctrl_reg2 &= ~0x20;
        }
        else
        {
            *bus_ptr->ssp_ctrl_reg2 |= 0x20;
        }
        *bus_ptr->ssp_ctrl_reg2 |= 0x10;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN DrvI2CCheckAck(DRVI2C_DEV_HNDL hndl)
{
    DRVI2C_DEV_PROPS* dev_ptr;
	I2C_BUS* bus_ptr;
	
	dev_ptr = &drvi2cdevice[hndl];
	bus_ptr = &drvi2cport[dev_ptr->ch];
    while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
    {
        asm("nop");//Timeout should be added here.
    }
    if((*bus_ptr->ssp_ctrl_reg2 & 0x40) == 0x40)    return FALSE;       //No Ack-bit was detected
    else                                            return TRUE;       //Ack-bit was detected
    return FALSE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvI2CBeginTransmission(DRVI2C_DEV_HNDL hndl, I2C_RW rw)
{
	DRVI2C_DEV_PROPS* dev_ptr;
	I2C_BUS* bus_ptr;
	
	dev_ptr = &drvi2cdevice[hndl];
	bus_ptr = &drvi2cport[dev_ptr->ch];
    
	//Startbit
	DrvI2CStart(hndl);
    
    while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
    {
        asm("nop");//Timeout should be added here.
    }
	//Send address
    if(rw == I2C_R) //RW-bit is set when reading
	{
        *bus_ptr->ssp_data_buf = dev_ptr->address | 0x01;
    }
    else if(rw == I2C_W) //RW-bit is cleared when writing
    {
        *bus_ptr->ssp_data_buf = dev_ptr->address & 0xFE;
    }
	while(((*bus_ptr->ssp_stat & 0x04) != 0) || ((*bus_ptr->ssp_ctrl_reg2 & 0x40) != 0))		//R/W-bit --> in master mode: transmission is in progress; Check ACK-bit
	{
		asm("nop");//Timeout should be added here.
	}
    while(((*bus_ptr->ssp_stat & 0x04) == 0x04)||((*bus_ptr->ssp_ctrl_reg2 & 0x1F) != 0x00))
    {
        asm("nop");//Timeout should be added here.
    }
}
/**********************************************************************************************************************/
