/**********************************************************************************************************************/
/**
 * @file        Std24LC512T.c
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
#define STD24LC512T_C
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
#include "..\..\Driver\PIC18F66K80\DrvI2C.h"
//STANDARD lib include section
#include "Std24LC512T.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
/* 24LC512T I2C ADDRESS*/
#define SLAVE_ADDR                    0x50
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
static DRVI2C_DEV_HNDL i2c_handle;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void Std24LC512TInit(void)
{
    i2c_handle =  DrvI2CRegisterDevice(I2C_CHANNEL_A, SLAVE_ADDR, 400000, I2C_MASTER); 
}
/*--------------------------------------------------------------------------------------------------------------------*/
void Std24LC512Clear(void)
{
    static unsigned char wdata, length = 128;
    unsigned int address = 0x0000;
    unsigned int data = 0xFF;
    unsigned int pagecntr = 0;     //64000 bytes / 128 bytes (max bytes possible to write/page)
    
    while(pagecntr < 500)          //64000 bytes / 128 bytes (max bytes possible to write/page)
    {
        DrvI2CBeginTransmission(i2c_handle, I2C_W);
        wdata = (char)(address >> 8);
        DrvI2CWriteMstr(i2c_handle, &wdata);        //Address High Byte
        wdata = (char)(address & 0x00FF);
        DrvI2CWriteMstr(i2c_handle, &wdata);        //Address Low Byte
    
        while(length--)
        {
            DrvI2CWriteMstr(i2c_handle, &data);
        }
        length = 128;
        DrvI2CStop(i2c_handle);
        do
        {
            DrvI2CStart(i2c_handle);
            DrvI2CWriteAddrMstr(i2c_handle, I2C_W);
        }
        while(DrvI2CCheckAck(i2c_handle) == FALSE);
        address += 0x80;
        pagecntr++;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void Std24LC512TWriteByte(unsigned int address, unsigned char byte)
{
    static unsigned char data;
    DrvI2CBeginTransmission(i2c_handle, I2C_W);
    data = (char)(address >> 8);
    DrvI2CWriteMstr(i2c_handle, &data);        //Address High Byte
    data = (char)(address & 0x00FF);
    DrvI2CWriteMstr(i2c_handle, &data);        //Address Low Byte
    DrvI2CWriteMstr(i2c_handle, &byte);
    DrvI2CStop(i2c_handle);
    
    do
    {
        DrvI2CStart(i2c_handle);
        DrvI2CWriteAddrMstr(i2c_handle, I2C_W);
    }
    while(DrvI2CCheckAck(i2c_handle) == FALSE);                             //Check if ACK-bit was received; If ACK-bit is received the write operation is done.    
}
/*--------------------------------------------------------------------------------------------------------------------*/
void Std24LC512TWritePage(unsigned int address, char* data, unsigned char length)
{   
    static unsigned char wdata;
    DrvI2CBeginTransmission(i2c_handle, I2C_W);
    wdata = (char)(address >> 8);
    DrvI2CWriteMstr(i2c_handle, &wdata);        //Address High Byte
    wdata = (char)(address & 0x00FF);
    DrvI2CWriteMstr(i2c_handle, &wdata);        //Address Low Byte
    
    while(length--)
    {
        DrvI2CWriteMstr(i2c_handle, data);
        data++;
    }
    DrvI2CStop(i2c_handle);
    do
    {
        DrvI2CStart(i2c_handle);
        DrvI2CWriteAddrMstr(i2c_handle, I2C_W);
    }
    while(DrvI2CCheckAck(i2c_handle) == FALSE);                             //Check if ACK-bit was received; If ACK-bit is received the write operation is done. 
}
/*--------------------------------------------------------------------------------------------------------------------*/
void Std24LC512TReadPage(unsigned int address, char* data, unsigned char length)
{
    static unsigned char wdata;
    DrvI2CBeginTransmission(i2c_handle, I2C_W);
    wdata = (char)(address >> 8);
    DrvI2CWriteMstr(i2c_handle, &wdata);        //Address High Byte
    wdata = (char)(address & 0x00FF);
    DrvI2CWriteMstr(i2c_handle, &wdata);        //Address Low Byte
    DrvI2CRepeatedStart(i2c_handle);
    DrvI2CWriteAddrMstr(i2c_handle, I2C_R);
    while(length--)
    {
        DrvI2CReadMstr(i2c_handle, data, FALSE);
        data++;
    }
    DrvI2CReadMstr(i2c_handle, data, TRUE);
}
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/


