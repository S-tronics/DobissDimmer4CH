/**********************************************************************************************************************/
/**
 * @file        StdDS2485.c
 *
 * @author      Stijn Vermeersch
 * @date        31.03.2022
 *
 * @brief       
 *
 *
 * \n<hr>\n
 * Copyright (c) 2022, S-tronics BV\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#define STDDS2485_C
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
#include "StdDS2485.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/

//Device Slave Address
#define SLAVE_ADDR 0x40

//1-Wire Timings (µs)
#define T_W0L           68
#define T_REC           6
#define T_RSTL          560
#define T_RSTH          560
#define T_OP            400
#define T_SEQ           10

//Device I2C-registers
#define WRITE_MEM    	0x96            //Write Memory
#define READ_MEM     	0x44            //Read Memory
#define READ_STATUS  	0xAA            //Read Status
#define READ_CONFIG  	0x52            //Read Configuration Register
#define WRITE_CONFIG 	0x99			//Write Configuration Register
#define MASTER_RESET 	0x62            //1-Wire reset
#define SCRIPT          0x88            //1-Wire script command
#define	ONEWIRE_SEARCH	0x11			//Perform a 1-Wire search Algoritm
#define	FULL_CMD_SEQ	0x57			//Full Command Sequence

#define SUCCESS         0xAA            //Execution of a command was a succes
#define FAILURE         0x22            //Execution of a command contains errors
//ErrorFlags
#define ERR_TIMEOUT     0x01    		//Timeout occured on One-Wire bus.

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/

typedef enum
{
    DS2485_IDLE,
    DS2485_RESET,
    DS2485_CONFIG,
    DS2485_SEARCHROM,
    DS2485_CMD_WRITE,
    DS2485_WAIT,
    DS2485_CMD_READ
}
DS2485_STATE;

typedef struct
{
    bool rw;
    unsigned char pdata[16];
    unsigned char length;
    unsigned int delay;
    unsigned char err;
    DS2485_STATE backup;
}
DS2485_CMD;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
void StdDS2485OneWireTimerTask(void);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static DRVI2C_DEV_HNDL i2c_handle;
static unsigned char reg = 0x00;
static unsigned char data = 0x00;
static unsigned char err = 0x00;
static unsigned char pdata[16];
static DS2485_CMD cmd;
static DS2485_STATE state = DS2485_IDLE;
static char ROM_ID[8][8];
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
void StdDS2485OneWireTimerTask(void)
{
    StdTaskStop(onewiretask);
    err |= ERR_TIMEOUT;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdDS2485Command(void)
{
    if(cmd.rw == false)                 //Write
    {
        DrvI2CBeginTransmission(i2c_handle, I2C_W);
        DrvI2CWriteMstrArr(i2c_handle, cmd.pdata, cmd.length);
        DrvI2CStop(i2c_handle);
    }
    else if(cmd.rw == true)
    {
        DrvI2CBeginTransmission(i2c_handle, I2C_R);
        DrvI2CReadMstrArr(i2c_handle, cmd.pdata, cmd.length);
    }
}
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void StdDS2485Init(void)
{       
    i2c_handle = DrvI2CRegisterDevice(I2C_CHANNEL_A, SLAVE_ADDR, 400000, I2C_MASTER);
    onewiretask = StdTaskRegisterTask(1000, (TASK_CALLBACK_FUNC)StdDS2485OneWireTimerTask, NULL, 128);
    state = DS2485_IDLE;
    //Configure 1-Wire timings
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdDS2485Config(void)
{
    const uint16_t  t_slot = T_W0L + T_REC;
    const uint16_t  ow_rst_time = T_RSTL + T_RSTH;
    const uint16_t  one_wire_time = ((t_slot * 8) * 64) + ow_rst_time;  
    static unsigned char flow = 1;
    if(flow == 1 && (state == DS2485_IDLE))
    {
        state = DS2485_CONFIG;
        flow = 1;
    }
    else
    {
        switch(flow)
        {
            case 1:
                cmd.backup = DS2485_CONFIG;
                cmd.rw = false;
                cmd.delay = T_OP + (T_SEQ * 1) + one_wire_time;
                cmd.pdata[0] = WRITE_CONFIG;
                cmd.pdata[1] = 3;           //Write length of bytes that are following
                cmd.pdata[2] = 0x00;        //Master configuration
                cmd.pdata[3] = 0x00;
                cmd.pdata[4] = 0x10;        //Enable APU
                cmd.length = 5;
                state = DS2485_CMD_WRITE;
                flow = 2;
                break;
            case 2:
                cmd.pdata[2] = 0x11;        //RPUP/BUF register
                cmd.pdata[3] = 0x06;        //Predefined value 6
                cmd.pdata[4] = 0x00;
                state = DS2485_CMD_WRITE;
                flow = 3;
                break;
            case 3:
                cmd.pdata[0] = SCRIPT;
                cmd.pdata[1] = 2;
                cmd.pdata[2] = 0x00;        //1-Wire reset
                cmd.pdata[3] = 0x80;
                cmd.length = 4;
                state = DS2485_CMD_WRITE;
                flow = 4;
                break;
            case 4:
                cmd.rw = true;
                cmd.length = 3;
                state = DS2485_CMD_READ;
                flow = 5;
                break;
            case 5:
                state = DS2485_SEARCHROM;
                break;
            default:
                break;
        }
    }
    
}
/*--------------------------------------------------------------------------------------------------------------------*/
char StdDS2485SearchRom(void)
{
    static unsigned char flow = 1;
    const uint16_t  t_slot = T_W0L + T_REC;
    const uint16_t  ow_rst_time = T_RSTL + T_RSTH;
    const uint16_t  one_wire_time = ((t_slot * 8) * 64) + ow_rst_time;  
    static unsigned char onwireindex = 0;
    unsigned char i = 0;
	
    if((flow == 0)&&(state == DS2485_IDLE))
    {
        state = DS2485_SEARCHROM;
        flow = 1;
        return 0;
    }
    else
    {
        switch(flow)
        {
            case 1:
                cmd.rw = false;
                cmd.pdata[0] = ONEWIRE_SEARCH;
                cmd.pdata[1] = 2;                       //Length of data to send
                cmd.pdata[2] = 0x05;                    //Parameter: 1-Wire reset; Find the first ROMID
                cmd.pdata[3] = 0xF0;                    //Search Rom command
                cmd.length = 4;
                cmd.delay = T_OP + T_SEQ * (64 + 1) + one_wire_time;
                cmd.backup = DS2485_SEARCHROM;
                state = DS2485_CMD_WRITE;
                flow = 2;
                onwireindex = 0;
                break;
            case 2:
                DrvI2CBeginTransmission(i2c_handle, I2C_R);
                DrvI2CReadMstr(i2c_handle, cmd.pdata, FALSE);
                if(cmd.pdata[0] == 10)
                {
                    DrvI2CReadMstr(i2c_handle, cmd.pdata, FALSE);
                    if(cmd.pdata[0] == SUCCESS)
                    {
                        for(i=0; i<8; i++)
                        {
                            DrvI2CReadMstr(i2c_handle, cmd.pdata, FALSE);
                            ROM_ID[onwireindex][i] = cmd.pdata[0];
                        }
                    }
                    onwireindex++;
                    DrvI2CReadMstr(i2c_handle, cmd.pdata, FALSE);
                    if(cmd.pdata[0] == 0x00)
                    {
                        state = DS2485_SEARCHROM;
                        flow = 3;
                    }
                    else
                    {
                        state = DS2485_IDLE;
                        flow = 1;
                    }
                }
                else
                {
                    DrvI2CReadMstr(i2c_handle, cmd.pdata, TRUE);
                    state = DS2485_RESET;
                    flow = 1;
                }
                break;  
            case 3:
                cmd.rw = false;
                cmd.pdata[0] = ONEWIRE_SEARCH;
                cmd.pdata[1] = 2;                       //Length of data to send
                cmd.pdata[2] = 0x01;                    //Parameter: 1-Wire reset; 
                cmd.pdata[3] = 0xF0;                    //Search Rom command
                cmd.length = 4;
                cmd.delay = T_OP + T_SEQ * (64 + 1) + one_wire_time;
                cmd.backup = DS2485_SEARCHROM;
                state = DS2485_CMD_WRITE;
                flow = 2;
                break;
            default:
                break;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
void StdDS2485MasterReset(void)
{
    static unsigned char flow = 0;
    if((flow == 0) && (state == DS2485_IDLE))
    {
        state = DS2485_RESET;
        flow = 1;
        return;
    }
    else
    {
        switch (flow)
        {
            case 1:
                cmd.rw = false;
                cmd.pdata[0] = MASTER_RESET;
                cmd.length = 1;
                cmd.delay = T_OP;
                cmd.backup = DS2485_RESET;
                state = DS2485_CMD_WRITE;
                flow = 2;
                break;
            case 2:
                cmd.rw = true;
                cmd.length = 2;             //Length + resultbyte
                cmd.backup = DS2485_RESET;
                state = DS2485_CMD_READ;
                flow = 3;
                break;
            case 3: 
                if(cmd.pdata[1] == SUCCESS)
                {
                    state = DS2485_CONFIG;
                    flow = 0;
                }
                else
                {
                    state = DS2485_RESET;       //Retry
                    flow = 1;
                }
                break;
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdDS2485Handler(void)
{
    switch(state)
    {
        case DS2485_IDLE:
            break;
        case DS2485_RESET:
            StdDS2485MasterReset();
            break;
        case DS2485_SEARCHROM:
            StdDS2485SearchRom();
            break;
        case DS2485_CONFIG:
            StdDS2485Config();
            break;
        case DS2485_CMD_WRITE:
            StdDS2485Command();
            StdTaskSetPeriod(onewiretask, cmd.delay);
            StdTaskStart(onewiretask);
            state = DS2485_WAIT;
            break;
        case DS2485_WAIT:
            if(!StdTaskIsTaskRunning(onewiretask))  
            {
                state = cmd.backup;
            }
            break;
        case DS2485_CMD_READ:
            StdDS2485Command();
            state = cmd.backup;
            break;
        default:
            break;
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/


