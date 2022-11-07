/*
 * File:   AppEeprom.c
 * Author: stronics
 *
 * Created on 03 August 2021, 13:11
 */

/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "AppConfig.h"
#include "..\Source\System\PIC18F66K80\SysLibAll.h"
//Driver Include Section
#include "..\Source\Driver\PIC18F66K80\DrvCan.h"
//Standard Include Section
#include "..\Source\Standard\I2C\Std24LC512T.h"
#include "..\Source\Standard\Timer\StdTask.h"
//Application Include Section
#include "AppDimmer.h"
#include "AppEeprom.h"
#include "AppLed.h"
#include "AppZCD.h"

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
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
void AppEepromTimerTask(void);
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static TASK_HNDL eepromtask;
/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppEepromTimerTask(void)
{
    StdTaskStop(eepromtask);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppEepromInit(void)
{
	eepromtask = StdTaskRegisterTask(10000, AppEepromTimerTask, NULL, 127);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppEepromreadMem(void)
{
    unsigned char i, j, k;
    char data[16];
    unsigned int address;
    // -----------------------------------------------
    // Algemene gegevens lezen uit EEPROM
    // -----------------------------------------------
    Std24LC512TReadPage(0x0000, data, 16);
    // Fill module configuration
    for (i = 0; i < 14; i++)            //Versie en type niet uitlezen...
    {
        module.all[i] = data[i];
    }
    if (module.time2function == 0xFF)
    {
        // At first boot of the module, this value is not set, so we set it to the default value and store this to EEPROM
        module.time2function = 2;
        Std24LC512TWritePage(0x0000, module.all, 16);
    }

    // -----------------------------------------------
    // Configuratie van 4 outputs lezen uit EEPROM
    // -----------------------------------------------
    address = 0x0040;
    for (i = 0; i < NBROFCHANNELS; i++) {
        // Read per 16 bytes (reading all 64 at once, makes the module crash - reason unknown)
        for (j = 0; j < 4; j++) {
            Std24LC512TReadPage(address + (j*0x10), data, 16);
            for (k = 0; k < 16; k++)
            {
                dim_ch[i].all[(j*16) + k] = data[k];
            }
        }
        
        // Now set the DIM settings in the correct RAM locations
        dim_ch[i].tUpdef = (dim_ch[i].tUp1 * 256) + dim_ch[i].tUp0;
        dim_ch[i].tDowndef = (dim_ch[i].tDown1 * 256) + dim_ch[i].tDown0;
        // Check VMax
        if (dim_ch[i].vMax < dim_ch[i].vMin || dim_ch[i].vMax == 0 || dim_ch[i].vMax > 100)
        {
            dim_ch[i].vMax = 100;
        }
        dim_ch[i].step = (dim_ch[i].vMax - dim_ch[i].vMin) / 10;   // Calculate the steps for 1%
        if (dim_ch[i].syncOutput > (NBROFCHANNELS - 1))
        {
            // Incorrect value, so we set it to 0xFF
            dim_ch[i].syncOutput = 0xFF;
        }

        // Set default values if the value are not initialized
        if (dim_ch[i].tStart == 0xFF)
        {
            // Change values in RAM
            //dim_ch[i].loadType = ind;
            dim_ch[i].tStart = 0;
            dim_ch[i].tUp1 = 0;
            dim_ch[i].tUp0 = 1;
            dim_ch[i].tUpdef = 1;
            dim_ch[i].tJump = 0;
            dim_ch[i].tDown1 = 0;
            dim_ch[i].tDown0 = 1;
            dim_ch[i].tDowndef = 1;
            dim_ch[i].vJump = 20;
            dim_ch[i].vStart = 0;
            dim_ch[i].vMin = 0;
            dim_ch[i].runSpeed = 1;
            dim_ch[i].vMax = 100;
            dim_ch[i].syncOutput = 0xFF;

            // Change values in Eeprom   
            for (k = 0; k < 16; k++)
            {
                data[k] = dim_ch[i].all[(3*16) + k];
            }
            //Delay10KTCYx(5); // 10ms
            StdTaskStart(eepromtask);
            while(StdTaskIsTaskRunning(eepromtask)) {};
            Std24LC512TWritePage(address + (3 * 0x10), data, 16);
        }
        
        address += 0x0040;
        if((address & 0x00FF) == 0x0000) address += 0x0100;
    }
    
    //writeSlaveSPI(0x02, DIM_CH[0].loadType, DIM_CH[1].loadType, DIM_CH[2].loadType, DIM_CH[3].loadType);              // Send values via SPI to the slave PIC18F25k80
    AppZCDConfig(0, dim_ch[0].ph, 0);
    AppZCDConfig(1, dim_ch[1].ph, 0);
    AppZCDConfig(2, dim_ch[2].ph, 0);
    AppZCDConfig(3, dim_ch[3].ph, 0);
    AppLEDControl(BUZZY, FALSE, RED);               // Buzzy led UIT

    DrvCanInit(module.address);
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/


