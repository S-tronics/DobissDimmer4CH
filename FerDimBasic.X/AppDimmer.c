/*
 * File:   AppDimmer.c
 * Author: stronics
 *
 * Created on 09 april 2021, 13:11
 */

/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "AppConfig.h"
#include "..\Source\System\PIC18F66K80\SysLibAll.h"
//Driver Include Section
#include "..\Source\Driver\PIC18F66K80\DrvGpio.h"
//Standard Include Section
#include "..\Source\Standard\Timer\StdTask.h"
#include "..\Source\Standard\I2C\StdDS2484.h"
#include "..\Source\Standard\I2C\Std24LC512T.h"
//Application Include Section
#include "AppConfig.h"
#include "AppDimmer.h"
#include "AppButton.h"
#include "AppLed.h"
#include "AppCan.h"
#include "AppEeprom.h"
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
void AppDimmerTimerTask(void);
void AppDimmerResetPeakValues(void);
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static TASK_HNDL dimmertask;
/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppDimmerTimerTask(void)
{
    StdTaskStop(dimmertask);
    
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppDimmerInit(void)
{
	unsigned char i, j;
    unsigned char test[5] = "*****";
    
    dimmertask = StdTaskRegisterTask(100000, AppDimmerTimerTask, NULL, 125);
    
    RTCminutes = 60;    
    module.all[16] = 0x00;             //Startup configuration
    module.all[17] = 0x00;          //Interrupt & buttons Scan
    AppLEDControl(POWER, FALSE, RED);
    AppLEDControl(BUZZY, FALSE, RED);
    AppLEDControl(CH1, FALSE, RED);
    AppLEDControl(CH1, FALSE, GREEN);
    AppLEDControl(CH2, FALSE, RED);
    AppLEDControl(CH2, FALSE, GREEN);
    
    //CMD_pulsvolger.Purp.follow = 0; //Pulsvolger = 0
    //CMD_pulsvolger.Purp.dim = 0;    //Dimmer via bus = 0
    

    
    //Loop through all 5 command lines
    for (i=0; i<5; i++)
    {
        // Initialize Purp struct values on 0
        cmd[i].Purp.flick = 0;
        cmd[i].Purp.follow = 0;
        cmd[i].Purp.dim = 0;
        // Reset DelayON and DelayOFF at startup (to prevent auto turning ON/OFF after autolearn)
        cmd[i].action.delayOn.all = 0xFF;
        cmd[i].action.delayOff.all = 0xFF;
    }
    // Initialize outputs
    for (i=0; i<NBROFCHANNELS; i++)
    {
        dim_ch[i].actValue = 0;
        dim_ch[i].moving = NO;
        dim_ch[i].direction = UP;      // by default start dim running UP    
        dim_ch[i].running = FALSE;
        dim_ch[i].led.onoff = FALSE;
    }
    // Initialize RTC counters
    for (i=0; i<NBROFCHANNELS; i++)
    {
        for (j=0; j<4; j++)
        {
            rtccount[i].all[j] = 0;
        }
    }
    
    // Initialize DIMrunning
    for (i=0; i<3; i++)
    {
        for (j=0; j<NBROFCHANNELS; j++) 
        {
            DIMrunning[i][j] = NO;
        }
    }
    
    StdDS2484DeviceReset();
    //StdDS2484WriteConfiguration(0b11100001);
    AppButtonclearProcessedButtons();
    // Write Version number to EEPROM location 0x000F
    module.version = 107;
    Std24LC512TWriteByte(0x0F, module.version);
    module.type = 0x10; 
    Std24LC512TWriteByte(0x0E, module.type); 
    
    // Read memory voor o.a. Address, config
    AppEepromreadMem();
    
    //Reset possible overcurrents
    AppZCDReset(0);
    AppZCDReset(1);
    AppZCDReset(2);
    AppZCDReset(3);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppDimmerReset(void)
{
    unsigned char i;

    for (i=0; i<NBROFCHANNELS; i++)
    {
        dim_ch[i].actValue = 0;        // Set all channels to 0
        dim_ch[i].moving = NO;         // Stop dim process
    }
    AppDimmerUpdate();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppDimmerControl(void)
{
    unsigned char i, dsChannel, update=0;
    
    for (i=0; i<NBROFCHANNELS; i++)                 // for each output channel
    {
        // <editor-fold defaultstate="collapsed" desc="Check busy Running through buttons pressed">
        for (dsChannel=0; dsChannel<3; dsChannel++)
        {
            if (!DIMrunning[dsChannel][i])              // not running
                continue;

            if (DIMrunning[dsChannel][i] == UP) 	// DIMrunning UP
            {
                if (!--dim_ch[i].speed)
                {
                    if (dim_ch[i].actValue == 0 && dim_ch[i].vStart > 0)
                    {
                        dim_ch[i].actValue = dim_ch[i].vStart;    // Start manual dimming from vStart instead of from 0
                    }
                    else
                    {
                        dim_ch[i].actValue++;
                    }
                    if (dim_ch[i].actValue >= 100)
                    {
                        dim_ch[i].actValue = 100;          // The value should not be more than 100%
                        DIMrunning[dsChannel][i] = DOWN;// run DOWN
                        dim_ch[i].direction = DOWN;
                    }
                    dim_ch[i].speed = dim_ch[i].runSpeed;
                    update = 1;
                }                
            }
            else if (DIMrunning[dsChannel][i] == DOWN) 	// DIMrunning DOWN
            {
                if (!--dim_ch[i].speed)
                {
                    dim_ch[i].actValue--;
                    if (dim_ch[i].actValue == 0 || dim_ch[i].actValue == 0xFF) // Value becomes 0 or 0xFF (when continuing in direction DOWN after the light was switched OFF)
                    {
                        dim_ch[i].actValue = 1;
                        DIMrunning[dsChannel][i] = UP;	// run UP
                        dim_ch[i].direction = UP;
                    }
                    dim_ch[i].speed = dim_ch[i].runSpeed;
                    update = 1;
                }
            }
            if (update && dim_ch[i].actValue % 10 == 0)
            {
                // Send status update of every 10% change
                AppCanSendStatusUpdate(i, dim_ch[i].actValue);
                update = 0;
            }
	}
        // </editor-fold>

    if (dim_ch[i].moving)
        {
            if (dim_ch[i].moving == UP)                                            // Dimming UP
            {
                // Go to end value with DIM_CH[i].speed = tUp
                if (!--dim_ch[i].speed)                                            // if speed = 0
                {
                    dim_ch[i].speed = dim_ch[i].tUp;
                    dim_ch[i].actValue++;
                    update = 1;

                    if (dim_ch[i].actValue == dim_ch[i].endValue)
                    {
                        dim_ch[i].moving = NO;
                    }
                }
            }
            else if (dim_ch[i].moving == DOWN)					// Dimming DOWN
            {
                // Go to end value with DIM_CH[i].speed = tDown
                if (!--dim_ch[i].speed)                                            // if speed = 0
                {
                    dim_ch[i].speed = dim_ch[i].tDown;
                    dim_ch[i].actValue--;
                    update = 1;

                    if (dim_ch[i].actValue == dim_ch[i].endValue)
                    {
                        dim_ch[i].moving = NO;
                    }
                }
            }
        }
    }

    if (update == 1)
    {
        AppDimmerUpdate();                                 // Update all dim channels
    }

}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppDimmerUpdate(void)
{
    unsigned char i, led;
    
    for (i=0; i<NBROFCHANNELS; i++)
    {
        if (dim_ch[i].actValue == 0)
        {
            dim_ch[i].led.color = GREEN;
            dim_ch[i].led.onoff = FALSE;
            
            dim_ch[i].realValue = 0;
        }
        else
        {
            dim_ch[i].led.color = GREEN;
            dim_ch[i].led.onoff = TRUE;
            // Calculate real value based on actual dimming range
            dim_ch[i].realValue = ((int)dim_ch[i].vMin * 10) + ((int)dim_ch[i].actValue * (int)dim_ch[i].step);
        }

    }
    
    INTCONbits.GIEL = 0;				// Global Interrupt bit
    
    AppZCDConfig(0, dim_ch[0].ph, dim_ch[0].realValue);
    AppZCDConfig(1, dim_ch[1].ph, dim_ch[1].realValue);
    AppZCDConfig(2, dim_ch[2].ph, dim_ch[2].realValue);
    AppZCDConfig(3, dim_ch[3].ph, dim_ch[3].realValue);
    
    if (!module.intBlock) INTCONbits.GIEL = 1;	// Global Interrupt bit
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/


