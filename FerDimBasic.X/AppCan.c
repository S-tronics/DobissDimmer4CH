/*
 * @file   		AppCan.c
 *
 * @author	 	Stijn Vermeersch
 *
 * @brief
 *
 * \n<hr>\n
 * Copyright (c) 2022, S-tronics BV\n
 * All rights reserved.
 * \n<hr>\n
 */

/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "AppConfig.h"
#include "..\Source\System\PIC18F66K80\SysLibAll.h"
//Driver Include Section
#include "..\Source\Driver\PIC18F66K80\DrvCan.h"
#include "..\Source\Driver\PIC18F66K80\DrvGpio.h"
//Standard Include Section
#include "..\Source\Standard\I2C\Std24LC512T.h"
#include "..\Source\Standard\Timer\StdTask.h"

//Application Include Section
#include "AppAction.h"
#include "AppScenarios.h"
#include "AppButton.h"
#include "AppDimmer.h"
#include "AppCan.h"
#include "AppLed.h"
#include "AppEeprom.h"
/**********************************************************************************************************************/

/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   T Y P E D E F S  &  E N U M S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
void AppCanTimerTask(void);
void AppCanWriteConfigFromCan(LOC loc);
void AppCanReadDataToCan(LOC loc);
void AppCanWriteDataFromCan(LOC loc);
void FlickerLed(void);
void AppCanReceiveCheck (APPCAN_MESSAGE* appmsg);

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static TASK_HNDL cantask;
static CAN_MESSAGE msg;
static APPCAN_MESSAGE locappmsg;
/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppCanTimerTask(void)
{
    StdTaskStop(cantask);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppCanWriteConfigFromCan(LOC loc)
{
    unsigned char dataToSave[16];
    static char data[8];
    unsigned char gpr, info;
    static unsigned char i, r, c;

	locappmsg.gpr = 2;
	locappmsg.type = 0x00;
	locappmsg.address = 0xFF;
	locappmsg.suffix = 0x06;
	locappmsg.data = data;
	locappmsg.length = 0;	
    AppCanSend(&locappmsg);
	
    for (r=0; r<loc.rowCount; r++)
    {
        for (c=0; c<loc.colDataCount / 8; c++)
        {
            if (AppCanReceiveNext(0x04, &locappmsg))
            {
                for (i=0; i<8; i++)
                {
                    //dataToSave[i + (c * 8)] = data[i];
                    dataToSave[i + (c * 8)] = locappmsg.data[i];
                }
            }
            else
            {
				locappmsg.gpr = 5;
				locappmsg.type = 0x00;
				locappmsg.info = 0;
				locappmsg.address = 0xFF;
				locappmsg.suffix = 0x11;
				locappmsg.data = data;
				locappmsg.length = 0;
                AppCanSend(&locappmsg);
                return;
            }
        }
        data[0] = r;
		locappmsg.gpr = 1;
		locappmsg.type = 0x00;
		locappmsg.info = 0;
		locappmsg.address = 0xFF;
		locappmsg.suffix = 0x06;
		locappmsg.data = data;
		locappmsg.length = 1;
        AppCanSend(&locappmsg);
		
        if (loc.location == 0 && loc.colDataCount == 16)
        {
            loc.colDataCount = 14;
            if (dataToSave[14] != module.type)                                     // Extra check when changing the address: type can't change!
            {
				locappmsg.gpr = 5;
				locappmsg.type = 0x00;
				locappmsg.info = 0;
				locappmsg.address = 0xFF;
				locappmsg.suffix = 0x06;		//Reply Write Data
				locappmsg.data = data;
				locappmsg.length = 0;
				AppCanSend(&locappmsg);
                return;
            }
        }
        Std24LC512TWritePage(loc.location, dataToSave, loc.colDataCount);
        if (module.address != dataToSave[0])
        {
            // If the address was changed, we will update the actions of all buttons
            
            AppButtonUpdateButtonsAddress(dataToSave[0]);
        }
        StdTaskSetPeriod(cantask, 6000);
        StdTaskStart(cantask);
        while(StdTaskIsTaskRunning(cantask)) {};
        //Delay10KTCYx(3);
        
        loc.location += loc.colMaxCount;
        if((loc.location & 0x00FF) == 0x00) loc.location += 0x0100;
    }
	locappmsg.gpr = 0;
	locappmsg.type = 0x00;
	locappmsg.info = 0;
	locappmsg.address = 0xFF;
	locappmsg.suffix = 0x06;		//Reply Write Data
	locappmsg.data = data;
	locappmsg.length = 0;
    AppCanSend(&locappmsg);
    //DrvCanSend(0, 0x00, 0, 0xFF, 0x06, data, 0);
    AppEepromreadMem();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppCanReadDataToCan(LOC loc)
{
    unsigned int r, c;
    static char data[64];
    //struct { unsigned x:1; } TEST;
    unsigned int rows;
    unsigned char initLow = loc.location & 0x00FF;
    //TEST.x = 1;
    if (loc.rowCount == 0)
        rows = 2000;                // onbeperkt - lees totdat 0xFF uit EEPROM gelezen wordt
    else
        rows = loc.rowCount;

    for (r = 0; r < rows; r++)
    {
        Std24LC512TReadPage(loc.location, data, loc.colDataCount);
        if (rows == 2000 && data[0] == 0xFF)
        {
			locappmsg.gpr = 0;
			locappmsg.type = 0x00,
			locappmsg.info = 0;
			locappmsg.address = 0xFF;
			locappmsg.suffix = 0x10;
			locappmsg.data = data;
			locappmsg.length = 0;
            AppCanSend(&locappmsg);
            break;
        }
        for (c=0; c<(loc.colDataCount / 8); c++)
        {
			locappmsg.gpr = (loc.colDataCount / 8) - c - 1;
			locappmsg.type = 0x00;
			locappmsg.info = 1;
			locappmsg.address = 0xFF;
			locappmsg.suffix = 0x10;		//Reply Read Data
			locappmsg.data = data + (c * 8);
			locappmsg.length = 8;
			AppCanSend(&locappmsg);
            StdTaskSetPeriod(cantask, 8000);
            StdTaskStart(cantask);
            while(StdTaskIsTaskRunning(cantask)) {};
            //Delay10KTCYx(4); // 8ms
        }
        StdTaskSetPeriod(cantask, 6000);
        StdTaskStart(cantask);
        while(StdTaskIsTaskRunning(cantask)) {};
        //Delay10KTCYx(3); // 6ms
        loc.location += loc.colMaxCount;
        if((loc.location & 0x00FF) == initLow) loc.location += 0x0100;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppCanWriteDataFromCan(LOC loc)
{
    unsigned char dataToSave[64];
    static char data[8];
    static unsigned char gpr;
    static unsigned char info;
    unsigned char i, r, c;
    unsigned int memLoc;
    //unsigned char memHigh, memLow;
    unsigned char order;
    
    AppLEDControl(POWER, FALSE, RED);
	locappmsg.gpr = 0;
	locappmsg.type = 0x00;
	locappmsg.info = 0;
	locappmsg.address = 0xFF;
	locappmsg.suffix = 0x11;				//Reply Write Data
	locappmsg.data = data;
	locappmsg.length = 0;
	AppCanSend(&locappmsg);

    memLoc = loc.location;
    for (r=0; r<loc.rowCount; r++)
    {
        DrvGpioSetPin(PORT_D, 6);
        
        // Flicker busy led
        //leds.bz = (leds.bz == 1) ? 0 : 1;
        AppLEDControl(BUZZY, TRUE, RED);
        //memHigh = memLoc >> 8;
        //memLow = memLoc;
        for (c=0; c<loc.colDataCount / 8; c++)
        {
            if (AppCanReceiveNext(0x11, &locappmsg))
            {
                order = ((loc.colDataCount / 8) - c - 1);
                if (locappmsg.gpr != order) 
                {
                    // Messages were received in incorrect order!
                    //data[0] = locappmsg.gpr;
                    //data[1] = order;
					locappmsg.gpr = 5;										//5, 0x00, 0, 0xDB, 0xFF, data, 2
					locappmsg.type = 0x00;
					locappmsg.info = 0;	
					locappmsg.address = 0xDB;
					locappmsg.suffix = 0xFF;                                //Reply Write Data
					//locappmsg.data = data;
					locappmsg.length = 8;
					AppCanSend(&locappmsg);
                    Nop();
                    return;
                }
                for (i=0; i<8; i++)
                {
                    dataToSave[i + (c * 8)] = locappmsg.data[i];
                }
            }
            else
            {
				locappmsg.gpr = 5;
				locappmsg.type = 0x00;
				locappmsg.info = 0;
				locappmsg.address = 0xFF;
				locappmsg.suffix = 0x11;				//Reply Write Data
				locappmsg.data = data;
				locappmsg.length = 0;
				AppCanSend(&locappmsg);
                return;
            }
            DrvGpioClearPin(PORT_D, 6);
        }
        data[0] = r;
        data[1] = memLoc >> 8;
        data[2] = memLoc & 0x00FF;
		locappmsg.gpr = 1;
		locappmsg.type = 0x00;
		locappmsg.info = 0;
		locappmsg.address = 0xFF;
		locappmsg.suffix = 0x11;				//Reply Write Data
		locappmsg.data = data;
		locappmsg.length = 3;
        AppCanSend(&locappmsg);
		
        if (memLoc == 0 && loc.colDataCount == 16)
        {
            loc.colDataCount = 14;
            if (dataToSave[14] != module.type)                                     // Extra check when changing the address: type can't change!
            {
				locappmsg.gpr = 5;
				locappmsg.type = 0x00;
				locappmsg.info = 0;
				locappmsg.address = 0xFF;
				locappmsg.suffix = 0x11;		//Reply Write Data
				locappmsg.data = data;
				locappmsg.length = 0;
				AppCanSend(&locappmsg);
                return;
            }
        }
        Std24LC512TWritePage(memLoc, dataToSave, loc.colDataCount);
        
        if (memLoc == 0 && module.address != dataToSave[0])
        {
            // If the address was changed, we will update the actions of all buttons
            AppButtonUpdateButtonsAddress(dataToSave[0]);
        }
        StdTaskSetPeriod(cantask, 6000);
        StdTaskStart(cantask);
        while(StdTaskIsTaskRunning(cantask)) {};
        //Delay10KTCYx(3);
        memLoc += loc.colMaxCount;
    }
	locappmsg.gpr = 0;
	locappmsg.type = 0x00;
	locappmsg.info = 0;
	locappmsg.address = 0xFF;
	locappmsg.suffix = 0x11;		//Reply Write Data
	locappmsg.data = (char*)data;
	locappmsg.length = 0;
	AppCanSend(&locappmsg);
    
    if ((loc.location == 0x0000) || (loc.location == 0x0040) || (loc.location == 0x0060))        // bij laden van Data & Configuratie
    {
        AppEepromreadMem();
    }
    AppLEDControl(BUZZY, FALSE, RED);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void FlickerLed(void)
{
    unsigned int i;

    for (i=0; i<1000; i++)
    {
//        asm("btg   LATA, 1, 0");
//        if (!MODE)
//        {
//            while (MODE == 0) {}
//            break;
//        }
        StdTaskSetPeriod(cantask, 50000);
        StdTaskStart(cantask);
        while(StdTaskIsTaskRunning(cantask)) {};
        //Delay10KTCYx(25);                       // 50ms
    }
    AppLEDControl(POWER, TRUE, RED);            // Power led AAN
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppCanReceiveCheck(APPCAN_MESSAGE* appmsg)
{
    unsigned char i, o;
	static APPCAN_MESSAGE locappmsg;
    static unsigned char out[8];
    LOC loc;
	
    switch (appmsg->suffix)
    {
        case 0x01:                      								// stuur status van de uitgangen terug
            if (!appmsg->info && appmsg->data[0] == module.address)
            {
                if (appmsg->data[1] < 4) {                                     // status van 1 uitgang werd opgevraagd
                    out[0] = dim_ch[appmsg->data[1]].actValue;
					locappmsg.gpr = 0;
					locappmsg.type = 0x7F;
					locappmsg.address = 0xFF;
					locappmsg.suffix = 0x01;
					locappmsg.data = (char*)out;
					locappmsg.length = 1;
                    AppCanSend(&locappmsg);							
                }
                if (appmsg->data[1] == 0xFF) {                              // status van alle uitgangen werd opgevraagd
                    for (i = 0; i<NBROFCHANNELS; i++) {                       // uitgang 1 t/m 4
                        out[i] = dim_ch[i].actValue;
                    }
                    for (i = 4; i < 8; i++) {                       // overige 4 bytes op 0xFF zetten
                        out[i] = 0xFF;
                    }
					locappmsg.gpr = 0;
					locappmsg.type = 0x00;
					locappmsg.info = 1;
					locappmsg.address = 0xFF;
					locappmsg.data = (char*)out;
					locappmsg.length = 8;
                    AppCanSend(&locappmsg);        
                }
            }
            break;
        case 0x02:	
            if (appmsg->data[0] == SCENARIOSADDR && appmsg->data[ACT_DELAYON] == 0xFF)
            {
                // A scenario CAN command
                //AppScenarioExecute(appmsg->data[1]);
                AppScenarioFillQueue(appmsg->data[1]);
            }
            else if (appmsg->data[0] == module.address)
            {
                // A CAN command for this module
                AppActionProcess(appmsg->data);
            }
            break;
        case 0x04:                                                              // Start (user will have to push on Service button) or Input Scan
            if (!appmsg->info && !appmsg->length && !module.toolWait)
            {
                module.toolScan = 1;
                module.toolTimer = 240;                                            // 240 sec to push button
            }
            //else if (info == 3)
            //{
            //    module.toolScan = 0;                                             // other module pushed first
            //    module.toolTimer = 0;
            //}
            if (module.toolWait)
            {
                module.toolWait = 0;
                                                                 
                AppLEDControl(POWER , TRUE, RED);                                   // Power led AAN
                loc.location = (((unsigned int)(appmsg->data[0])) << 8) + appmsg->data[1];
                loc.colMaxCount = appmsg->data[2];
                loc.rowCount = appmsg->data[3];
                loc.colDataCount = appmsg->data[4];
                AppCanWriteConfigFromCan(loc);
            }
            break;
        case 0x05:                                                              // Stop Scan
            module.toolScan = 0;
            module.toolWait = 0;
            module.toolTimer = 0;
            break;
        case 0x10:    
            loc.location = (((unsigned int)(appmsg->data[0])) << 8) + appmsg->data[1];  
            loc.colMaxCount = appmsg->data[2];
            loc.rowCount = appmsg->data[3];
            loc.colDataCount = appmsg->data[4];
            AppCanReadDataToCan(loc);
            break;
        case 0x11:                                                              // Write Data
            if (appmsg->length == 5) 
            {
                // Only start writing data from CAN if we receive a header message of 5 bytes first
                loc.location = (((unsigned int)(appmsg->data[0])) << 8) + appmsg->data[1];  
                loc.colMaxCount = appmsg->data[2];
                loc.rowCount = appmsg->data[3];
                loc.colDataCount = appmsg->data[4];
                AppCanWriteDataFromCan(loc);
            }
            break;
        case 0x0A:                                                              // Visuale detectie
            FlickerLed();
            break;
        case 0x0B:                                                              // Read details from MASTER
            if (module.master)
            {
                loc.location = (((unsigned int)(appmsg->data[0])) << 8) + appmsg->data[1]; 
                loc.colMaxCount = appmsg->data[2];
                loc.rowCount = appmsg->data[3];
                loc.colDataCount = appmsg->data[4];
                AppCanReadDataToCan(loc);
            }
            break;
        case 0x20:                                                              // Start button scan
            module.buttonScan = 1;
            AppButtonclearProcessedButtons();
            break;
        case 0x21:                                                              // Stop button scan
            module.buttonScan = 0;
            break;
        case 0xF1:                                                              // Set Bootloader actif
            //Bootloader();
            break;
        case 0xF2:                                                              // Reset module
            //_asm
            //    reset
            //_endasm
            break;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppCanInit(void)
{
	cantask = StdTaskRegisterTask(200, AppCanTimerTask, NULL, 127);
    
    DrvGpioInitPin(PORT_D, 6, PIN_IO_FUNC_OUTPUT_LOW);
    
    DrvCanInit(module.address);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppCanSendStatusUpdate(unsigned char output, unsigned char value)
{
    static char data[3];

    data[0] = module.address;
    data[1] = output;
    data[2] = value;
	locappmsg.gpr = 0;
	locappmsg.type = 0x00;
	locappmsg.info = 2;
	locappmsg.address = 0xFF;
	locappmsg.suffix = 0x01;			//Status
	locappmsg.data = (char*)data;
	locappmsg.length = 3;
	AppCanSend(&locappmsg);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppCanSend(APPCAN_MESSAGE* appmsg)
{
	unsigned long id;
	unsigned char i;
    
	msg.id.id = ((unsigned long)(appmsg->gpr)<<25) + ((unsigned long)(appmsg->type)<<18) + ((unsigned long)(appmsg->info)<<16) + ((unsigned long)(appmsg->address)<<8) + appmsg->suffix;
	//if(dataLen > 8) dataLen = 8;
	msg.dlc = appmsg->length;
	for(i = 0; i < appmsg->length; i++)
    {
        msg.data[i] = *appmsg->data++;
    }
	DrvCanSend(&msg);
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool AppCanRead(APPCAN_MESSAGE* appmsg)
{
    //DrvCanRxISR();
    if(DrvCanRead(&msg))
    {
        appmsg->gpr = (unsigned char)(msg.id.id >> 25);
        appmsg->info = (unsigned char)(msg.id.id >> 16) & 0x03;
        appmsg->type = (unsigned char)(msg.id.id >> 18);
        appmsg->suffix = (unsigned char)(msg.id.id & 0x000000FF);
        appmsg->length = msg.dlc;
        appmsg->data = msg.data;
        return true;
    }
    return false;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppCanHandler(void)
{
    
    if(AppCanRead(&locappmsg))
    {
        AppCanReceiveCheck(&locappmsg);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppCaneeReadConfigToCan(void)
{
    static unsigned char retData[8];
    unsigned char i;

    // suffix=0x04 --> reply Read Scan
    // suffix=0x06 --> reply Read Config
    for (i=0; i<8; i++)
        retData[i] = module.all[i];
	locappmsg.gpr = 1;
	locappmsg.type = 0x00;
	locappmsg.info = 3;
	locappmsg.address = 0xFF;
	locappmsg.suffix = 0x04;			//Reply Read Config
	locappmsg.data = (char*)retData;
	locappmsg.length = 8;
	AppCanSend(&locappmsg);
    for (i=0; i<8; i++)
        retData[i] = module.all[i + 8];
	locappmsg.gpr = 0;
    locappmsg.data = (char*)retData;
	AppCanSend(&locappmsg);
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool AppCanReceiveNext(unsigned char suffix, APPCAN_MESSAGE* msg)
{
    StdTaskSetPeriod(cantask, 4000000);              
    StdTaskStart(cantask);
    while(StdTaskIsTaskRunning(cantask))
    {
        //DrvCanRxISR();
        if(AppCanRead(msg))
        {
            if(msg->suffix == suffix)
            {
                StdTaskStop(cantask);
                return true;
            }
        }
    }
    return false;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/


