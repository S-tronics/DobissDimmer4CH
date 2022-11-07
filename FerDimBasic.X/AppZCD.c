/*
 * File:   AppZCD.c
 * Author: stronics
 *
 * Created on 09 april 2021, 13:11
 */

/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "..\Source\System\PIC18F66K80\SysLibAll.h"
#include "AppConfig.h"
//Driver Include Section
#include "..\Source\Driver\PIC18F66K80\DrvTimer.h"
#include "..\Source\Driver\PIC18F66K80\DrvCCP.h"
#include "..\Source\Driver\PIC18F66K80\DrvGpio.h"
//Standard Include Section

//Application Include Section
#include "AppConfig.h"
#include "AppDimmer.h"
#include "AppZCD.h"
#include "AppIrq.h"
/**********************************************************************************************************************/

/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/


#define     NBR_SMPL_STARTUP        20
#define     NBR_SMPL                100

#define     ZC_MASK                 5

#define     INT_INTEDG1             0x20
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
typedef enum
{
    UNKNOWN,
    EVALUATING,
    IDLE
}ZCD_STARTUP;
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/


static unsigned char    zcd_ctr = 0;
static bool             started = false;    
static unsigned int     ar_zcd_startup[NBR_SMPL_STARTUP];
static unsigned int     ar_zcd[NBR_SMPL];
static unsigned long    zcd_period = 0;
static unsigned long    zcd_period_calc = 0;
static unsigned long    zcd_period_min = 0;
static unsigned long    zcd_period_max = 0;
static bool             calc_zcd_period = false;
//static DIM_CHANNEL      channel[NBROFCHANNELS];
#ifdef NBR_CH_4
static DRVCCP_CH_HNDL   hndl_ch2 = 2;
static DRVCCP_CH_HNDL   hndl_ch3 = 3;
#endif
/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppZCDFlywheelStartup(void)
{
    static ZCD_STARTUP startupstate = UNKNOWN;
    
    switch(startupstate)
    {
        case UNKNOWN:
            startupstate = EVALUATING;      //Due to the unknown triggerpoint of the period and the startup time.
            drvtmr_tick_count = 0;          //Search for the first ZCD-pulse
            zcd_period_calc = 0;
            INTCON3bits.INT1IF = 0;
            break;
        case EVALUATING:                    //Now measure the first half periods to start-up the flywheel
            if(zcd_ctr < NBR_SMPL_STARTUP)
            {
                ar_zcd[zcd_ctr] = drvtmr_tick_count;              //Time for 1 half period
                zcd_period_calc += ar_zcd[zcd_ctr];
                zcd_ctr++;
            }
            else
            {
                zcd_ctr = 0;
                zcd_period_calc /= NBR_SMPL_STARTUP;
                zcd_period = zcd_period_calc;
                if((zcd_period > 55) && (zcd_period < 166))       //90Hz - 30Hz
                {
                    zcd_period_min = zcd_period_calc - ZC_MASK;
                    zcd_period_max = zcd_period_calc + ZC_MASK;
                    started = TRUE;
                    startupstate = IDLE;
                }
                else
                {
                    zcd_period = 0;
                    zcd_period_calc = 0;
                }
            }
            drvtmr_tick_count = 0;
            break;
        case IDLE:
            break;
        default:
            zcd_ctr = 0;
            startupstate = UNKNOWN;
            INTCON3bits.INT1IF = 0;
            break;
    }
}
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppZCDInit(void)
{
    unsigned char i = 0;
    DIM_CH* chptr;
    
    DrvGpioInitPin(PORT_B, 1, PIN_IO_FUNC_INPUT);           //Initialize ZCD-pin (RB1)(INT1)
    
    dim_ch[0].ch = DrvCCPRegisterChannel(0, CH_CCP2);
    dim_ch[1].ch = DrvCCPRegisterChannel(1, CH_CCP3);
    dim_ch[2].ch = DrvCCPRegisterChannel(2, CH_CCP4);
    dim_ch[3].ch = DrvCCPRegisterChannel(3, CH_CCP5);
    dim_ch[0].port_reset = PORT_B;                         //RD# Channel 0 (Reset Overcurrent)
    dim_ch[0].pin_reset = 4;
    dim_ch[1].port_reset = PORT_C;                         //RD# Channel 1 (Reset Overcurrent)
    dim_ch[1].pin_reset = 5;
    dim_ch[0].port_overcurrent = PORT_D;
    dim_ch[0].pin_overcurrent = 0;
    dim_ch[1].port_overcurrent = PORT_A;
    dim_ch[1].pin_overcurrent = 5;
    dim_ch[0].port_overtemperature = PORT_D;
    dim_ch[0].pin_overtemperature = 1;
    dim_ch[1].port_overtemperature = PORT_A;
    dim_ch[1].pin_overtemperature = 1;
    
    //Initialise the 4 Dimming channels.
    for(i=0; i<NBROFCHANNELS; i++)
    {
        chptr = &dim_ch[i];
        chptr->en = false;
        chptr->overcurrent = false;
        chptr->overtemperature = false;
        DrvGpioInitPin(chptr->port_reset, chptr->pin_reset, PIN_IO_FUNC_OUTPUT_HIGH);               //Initialise Reset-pin
        DrvGpioInitPin(chptr->port_overcurrent, chptr->pin_overcurrent, PIN_IO_FUNC_INPUT);         //Initialise Overcurrent-pin
        DrvGpioInitPin(chptr->port_overtemperature, chptr->pin_overtemperature, PIN_IO_FUNC_INPUT); //Initialise Overtemperature-pin
        DrvGpioTogglePin(chptr->port_reset, chptr->pin_reset);
        DrvGpioTogglePin(chptr->port_reset, chptr->pin_reset);
    }
    
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
void AppZCDChannelEnable(unsigned char index, bool en)
{
    if(index < NBROFCHANNELS)
    {
        dim_ch[index].en = en;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppZCDConfig(unsigned char index, PH_CTRL ph, unsigned int value)
{
    unsigned long     period;
    
    if((index < NBROFCHANNELS) && (started))
    {
        dim_ch[index].ph = ph;
        dim_ch[index].realValue = value;

        switch(ph)
        {
            case LEADING:
                if(value <= 1000)
                {
                    period = (zcd_period * 125 * value) / 1000;
                }
                else
                {
                    period = zcd_period * 125;
                }
                DrvCCPUpdate(dim_ch[index].ch, period);
                break;
            case TRAILING:
                if(value <= 1000)
                {
                    period = (zcd_period * 125)-(zcd_period * 125 * value)/1000;
                }
                else
                {
                    period = zcd_period * 125;
                }
                DrvCCPUpdate(dim_ch[index].ch, period);
                break;
            default:
                break;
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppZCDHandler(void)
{
    unsigned char i = 0;       
    DIM_CH* ch;
    if(calc_zcd_period)
    {
        zcd_period_calc = 0;
        for(i = 0; i < NBR_SMPL; i++)
        {
            zcd_period_calc += ar_zcd[i];
        }
        zcd_period_calc /= NBR_SMPL;
        zcd_period = zcd_period_calc;
        zcd_period_min = zcd_period_calc - ZC_MASK;
        zcd_period_max = zcd_period_calc + ZC_MASK;
        calc_zcd_period = FALSE;
    }
    for(i = 0; i < NBROFCHANNELS; i++)
    {
        ch = &dim_ch[i];
        if(DrvGpioIsPinHigh(ch->port_overcurrent, ch->pin_overcurrent)) 
        {
            ch->overcurrent = true;
            ch->en = false;
            AppZCDChannel(ch->ch);
        }
        else    ch->overcurrent = false;
        if(DrvGpioIsPinHigh(ch->port_overtemperature, ch->pin_overtemperature)) 
        {
            ch->overtemperature = true;
            ch->en = false;
            AppZCDChannel(ch->ch);
        }
        else    ch->overtemperature = false;
    }
    
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool AppZCDGetConfigurated(void)
{
    return started;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppZCDFlywheel(void)
{
    unsigned char i;
    if(!started)    AppZCDFlywheelStartup();
    else
    {
            if(zcd_ctr >= NBR_SMPL) 
            {
                calc_zcd_period = TRUE;
                zcd_ctr = 0;
            }
            if((drvtmr_tick_count > zcd_period_min) && (drvtmr_tick_count < zcd_period_max))
            {
                ar_zcd[zcd_ctr] = drvtmr_tick_count;
                zcd_ctr++;
                for(i = 0; i < NBROFCHANNELS; i++)
                {
                    if(dim_ch[i].en)
                    {
                        if(dim_ch[i].ph == LEADING)        DrvCCPStart(dim_ch[i].ch, CCP_ACTIVE_H);
                        else if(dim_ch[i].ph == TRAILING)  DrvCCPStart(dim_ch[i].ch, CCP_ACTIVE_L);
                    }
                }
                DrvCCPStartTimer(dim_ch[0].ch);
                drvtmr_tick_count = 0;
                //INTCON2bits.INTEDG1 = !INTCON2bits.INTEDG1; 
                //if(INTCON2bits.INTEDG1 == 1)    INTCON2bits.INTEDG1 = 0;
                //else                            INTCON2bits.INTEDG1 = 1;
                //if(INTCON2 & INT_INTEDG1)   INTCON2 &= ~INT_INTEDG1;    //Edge of INT1 on falling edge
                //else                        INTCON2 |= INT_INTEDG1;     //Edge of INT1 on rising edge
                
                //INTCON3 &= ~INT_INT1IF;         //Clear External Interrupt INT1 Flag
            }
            else
            {
                drvtmr_tick_count = 0;
            }  
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppZCDTimer(void)
{
    unsigned char i;
    if(INTCON3bits.INT1IF == 0)
    {
        if((drvtmr_tick_count > zcd_period_min) && (drvtmr_tick_count < zcd_period_max)) 
        {
            for(i=0; i < NBROFCHANNELS; i++)
            {
                if(dim_ch[i].en)
                {
                    
                    if(dim_ch[i].ph == LEADING)        DrvCCPStart(dim_ch[i].ch, CCP_ACTIVE_H);
                    else if(dim_ch[i].ph == TRAILING)  DrvCCPStart(dim_ch[i].ch, CCP_ACTIVE_L);
                }
            }
            DrvCCPStartTimer(dim_ch[0].ch);
            drvtmr_tick_count = 0;
            INTCON2bits.INTEDG1 = !INTCON2bits.INTEDG1; 
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppZCDChannel(unsigned char ch)
{
    if(ch < NBROFCHANNELS)
    {
        DrvCCPStop(dim_ch[ch].ch);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppZCDReset(unsigned char ch)
{
    if(ch < NBROFCHANNELS)
    {
        DrvGpioTogglePin(dim_ch[ch].port_reset, dim_ch[ch].pin_reset);
        DrvGpioTogglePin(dim_ch[ch].port_reset, dim_ch[ch].pin_reset);
        dim_ch[ch].overcurrent = false;
        dim_ch[ch].overtemperature = false;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool AppZCDGetOverCurrent(unsigned char ch)
{
    if(ch < NBROFCHANNELS)
    {
        return dim_ch[ch].overcurrent;
    }
    return true;
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool AppZCDGetOverTemperature(unsigned char ch)
{
    if(ch < NBROFCHANNELS)
    {
        return dim_ch[ch].overtemperature;
    }
    return true;
}
/**********************************************************************************************************************/


