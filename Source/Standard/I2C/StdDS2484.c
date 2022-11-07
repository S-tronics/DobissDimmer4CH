/**********************************************************************************************************************/
/**
 * @file        StdDS2484.c
 *
 * @author      Stijn Vermeersch
 * @date        27.07.2020
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
#define STDDS2484_C
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
#include "..\..\Standard\Timer\StdTask.h"
#include "StdDS2484.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/

//Device Slave Address
#define SLAVE_ADDR 0x18

//Device I2C-registers
#define DRST    0xF0            //Device Reset
#define SRP     0xE1            //Set Read Pointer
#define WCFG    0xD2            //Write Device Configuration
#define ADJP    0xC3            //Adjust 1-Wire Port

#define ONEWRS  0xB4            //1-Wire reset
#define ONEWSB  0x87            //1-Wire Single Bit
#define ONEWWB  0xA5            //1-Wire Write Byte
#define ONEWRB  0x96            //1-Wire Read Byte
#define ONEWT   0x78            //1-Wire Triplet

//ErrorFlags
#define ERR_TIMEOUT     0x01    //Timeout occured on One-Wire bus.

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
static void StdOneWireTimerTask(void);
void StdDS2484CommandCode(unsigned char cmd, unsigned char databyte, bool withdata);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static DRVI2C_DEV_HNDL i2c_handle;
static char reg = 0x00;
static char data = 0x00;
static unsigned char err = 0x00;
/* bit0 = time-out flag on one-wire executed command*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static TASK_HNDL onewiretask;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
static void StdOneWireTimerTask(void)
{
    StdTaskStop(onewiretask);
    err |= ERR_TIMEOUT;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdDS2484CommandCode(unsigned char cmd, unsigned char databyte, bool withdata)
{
    DrvI2CBeginTransmission(i2c_handle, I2C_W);				//Startbit + write address
    reg = cmd;
    DrvI2CWriteMstr(i2c_handle, &reg);
    if(withdata)    
    {
        data = databyte;
        DrvI2CWriteMstr(i2c_handle, &data);
    }
    DrvI2CRepeatedStart(i2c_handle);
    DrvI2CWriteAddrMstr(i2c_handle, I2C_R);
    DrvI2CReadMstr(i2c_handle, &data, FALSE);            //Read State Register
    StdTaskSetPeriod(onewiretask, 2000);                //Set Period to 2000 µs.
    StdTaskStart(onewiretask);
    while((data & 0x01) && (!(err & 0x01))){                                //Check for 1-Wire Busy flag in State Register.
        DrvI2CReadMstr(i2c_handle, &data, FALSE);
    }
    StdTaskStop(onewiretask);
    DrvI2CReadMstr(i2c_handle, &data, TRUE);
    
}
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void StdDS2484Init(void)
{       
    i2c_handle = DrvI2CRegisterDevice(I2C_CHANNEL_A, SLAVE_ADDR, 400000, I2C_MASTER);
    onewiretask = StdTaskRegisterTask(1000, (TASK_CALLBACK_FUNC)StdOneWireTimerTask, NULL, 121);
}
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char StdDS2484Write(unsigned char i2creg, unsigned char i2cdata, unsigned char wait)
{    
    DrvI2CBeginTransmission(i2c_handle, I2C_W);
    reg = i2creg;
    DrvI2CWriteMstr(i2c_handle, &reg);
    data = i2cdata;
    DrvI2CWriteMstr(i2c_handle, &data);
    DrvI2CStop(i2c_handle);
    DrvI2CBeginTransmission(i2c_handle, I2C_R);
    if(wait == 2)
    {
        StdTaskStart(onewiretask);
        do
        {
        DrvI2CReadMstr(i2c_handle, &data, FALSE);
        }
        while((data & 0x01)&&(StdTaskIsTaskRunning(onewiretask)));
    }
    DrvI2CReadMstr(i2c_handle, &data, TRUE);
    DrvI2CStop(i2c_handle);
    return data;
}
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char StdDS2484Read(unsigned char i2creg)
{
    //Set Read Pointer
    DrvI2CBeginTransmission(i2c_handle, I2C_W);				//Startbit + write address
    reg = SRP;
	DrvI2CWriteMstr(i2c_handle, &reg);
    reg = i2creg;
    DrvI2CWriteMstr(i2c_handle, &reg);
    DrvI2CStop(i2c_handle);
    //Read selected register
    DrvI2CBeginTransmission(i2c_handle, I2C_W);				//Startbit + write address
    DrvI2CReadMstr(i2c_handle, &data, TRUE);
    DrvI2CStop(i2c_handle);
    return data;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdDS2484DeviceReset(void)
{
    DrvI2CBeginTransmission(i2c_handle, I2C_W);				//Startbit + write address
    reg = DRST;
    DrvI2CWriteMstr(i2c_handle, &reg);
    DrvI2CRepeatedStart(i2c_handle);
    DrvI2CWriteAddrMstr(i2c_handle, I2C_R);
    DrvI2CReadMstr(i2c_handle, &data, TRUE);
    DrvI2CStop(i2c_handle);
    StdDS2484Write(WCFG, 0b11100001, 1);                       // Write Configuration (1WS=0, SPU=0, ***=0, APU=1)
    //return data;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdDS2484WriteConfiguration(unsigned char data)
{
    StdDS2484Write(WCFG, data, 1);                       // Write Configuration (1WS=0, SPU=0, PDN=0, APU=0)
}
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char StdDS2484OneWireWriteByte(unsigned char databyte, unsigned char* statereg)
{
    StdDS2484CommandCode(ONEWWB, databyte, true);
    *statereg = data;
    return err;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* One-Wire Reset Timing for DS2401
 * 
 * tRSTL = min. 480µs
 * tRSTH = min. 480µs
 */
unsigned char StdDS2484OneWireReset(unsigned char* statereg)
{
    StdDS2484CommandCode(ONEWRS, 0, false);
    *statereg = data;
    return err;
}
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char StdDS2484OneWireTriplet(unsigned char databyte, unsigned char* statereg)
{
    StdDS2484CommandCode(ONEWT, databyte, true);
    *statereg = data;
    return err;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/


