/**********************************************************************************************************************/
/**
 * @file        ccp\DrvCCP.c
 *
 * @author      Stijn Vermeersch
 * @date        02.04.2021
 *
 * @brief       Basic CCP manipulation functionality
 *
 * The CCP-module is used together with a timer to perform Capture Compare functionality.
 *
 * \n<hr>\n
 * Copyright (c) 2021, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#define CCP__DRVCCP_C
/**********************************************************************************************************************/



/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/
//#include "AppConfig.h"

/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "..\..\System\PIC18F66K80\SysLibAll.h"

//DRIVER lib include section
#include "DrvGpio.h"
//APPLICATION lib include section
#include "DrvCCP.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define CCP_CH_COUNT    4
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
typedef struct
{
	volatile unsigned char*	ccp_ctrl;		//Capture Compare Control register
	volatile unsigned char* 	ccp_tmrs;		//Capture Compare Timer Select register
	volatile unsigned char*	ccp_period_l;	//Capture Compare Period Low byte register
	volatile unsigned char*	ccp_period_h;	//Capture Compare Period High byte register
    volatile unsigned char*    ccp_odcon;      //Open drain control register
    volatile unsigned char*    ccp_tmr_ctrl;   //Timer control register
}
CCP_REG;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvCPPInitChannel(unsigned char chnr);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static CCP_CHANNEL      drvcppchannel[CCP_CH_COUNT];
static CCP_REG          drvcppreg[CCP_CH_COUNT];

static DRVCCP_CH_HNDL   drvccp_registered_chcount;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvCPPInitChannel(unsigned char chnr)
{
    CCP_REG* ccpregptr;
     
    switch(drvcppchannel[chnr].pin)
    {
        case CH_CCP1:
            break;
        case CH_CCP2:
            TRISC &= 0xFB;              //RC2 - CCP2
            ccpregptr = &drvcppreg[chnr];
            drvcppreg[chnr].ccp_ctrl = &CCP2CON;
            drvcppreg[chnr].ccp_period_h = &CCPR2H;
            drvcppreg[chnr].ccp_period_l = &CCPR2L;
            drvcppreg[chnr].ccp_odcon = &ODCON;
            drvcppreg[chnr].ccp_tmrs = &CCPTMRS;
            drvcppreg[chnr].ccp_tmr_ctrl = &T3CON;
            //*ccpregptr->ccp_odcon |= 0x08;
            break;
        case CH_CCP3:                      
            TRISC &= 0xBF;              //RC6 - CCP3
            ccpregptr = &drvcppreg[chnr];
            drvcppreg[chnr].ccp_ctrl = &CCP3CON;
            drvcppreg[chnr].ccp_period_h = &CCPR3H;
            drvcppreg[chnr].ccp_period_l = &CCPR3L;
            drvcppreg[chnr].ccp_odcon = &ODCON;
            drvcppreg[chnr].ccp_tmrs = &CCPTMRS;
            drvcppreg[chnr].ccp_tmr_ctrl = &T3CON;
            //*ccpregptr->ccp_odcon |= 0x10;
            break;
        case CH_CCP4:
            TRISC &= 0x7F;              //RC7
            ccpregptr = &drvcppreg[chnr];
            drvcppreg[chnr].ccp_ctrl = &CCP4CON;
            drvcppreg[chnr].ccp_period_h = &CCPR4H;
            drvcppreg[chnr].ccp_period_l = &CCPR4L;
            drvcppreg[chnr].ccp_odcon = &ODCON;
            drvcppreg[chnr].ccp_tmrs = &CCPTMRS;
            drvcppreg[chnr].ccp_tmr_ctrl = &T3CON;
            //*ccpregptr->ccp_odcon |= 0x20;
            break;
        case CH_CCP5:                      
            TRISB &= 0xDF;               //RB5
            ccpregptr = &drvcppreg[chnr];
            drvcppreg[chnr].ccp_ctrl = &CCP5CON;
            drvcppreg[chnr].ccp_period_h = &CCPR5H;
            drvcppreg[chnr].ccp_period_l = &CCPR5L;
            drvcppreg[chnr].ccp_odcon = &ODCON;
            drvcppreg[chnr].ccp_tmrs = &CCPTMRS;
            drvcppreg[chnr].ccp_tmr_ctrl = &T3CON;
            //*ccpregptr->ccp_odcon |= 0x40;
            break;
        default:
            break;
    }
    
    //if(pol == CCP_ACTIVE_H)         *ccpregptr->ccp_ctrl = 0x08;        //force CCP-pin high on compare match
    //else if(pol == CCP_ACTIVE_L)    *ccpregptr->ccp_ctrl = 0x09;        //force CCP-pin low on compare match 
    *ccpregptr->ccp_ctrl &= 0xF0;
    *ccpregptr->ccp_tmr_ctrl &= 0x3F;                                   //System clock = Fosc/4
    *ccpregptr->ccp_tmr_ctrl |= 0x20;                                   //Prescaler value = 1:4
    *ccpregptr->ccp_tmr_ctrl &= 0xF7;                                   //Clear SOSCEN Oscillator Enable bit
    
    //*ccpregptr->ccp_tmrs |= (1 << (unsigned char)pin_name);                //Select TMR3 for all CCP-outputs
    *ccpregptr->ccp_tmrs |= (1 << drvcppchannel[chnr].pin);             //Select TMR3 for all CCP-outputs
    *ccpregptr->ccp_period_h &= 0x00;                                   //Clear the period registers
    *ccpregptr->ccp_period_l &= 0x00;
}
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvCCPInit(void)
{
	drvccp_registered_chcount = 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DRVCCP_CH_HNDL DrvCCPRegisterChannel(unsigned char chnr, CCP_PIN pin_name)
{
    if(chnr == drvccp_registered_chcount)
    {
        drvcppchannel[drvccp_registered_chcount].pin = pin_name;
        drvcppchannel[drvccp_registered_chcount].chnr = chnr;
        switch(pin_name)
        {
            case CH_CCP2: 
                drvcppchannel[drvccp_registered_chcount].port = PORT_C;
                drvcppchannel[drvccp_registered_chcount].pinnr = 2;
                break;
            case CH_CCP3:
                drvcppchannel[drvccp_registered_chcount].port = PORT_C;
                drvcppchannel[drvccp_registered_chcount].pinnr = 6;
                break;
            case CH_CCP4:
                drvcppchannel[drvccp_registered_chcount].port = PORT_C;
                drvcppchannel[drvccp_registered_chcount].pinnr = 7;
                break;
            case CH_CCP5:
                drvcppchannel[drvccp_registered_chcount].port = PORT_B;
                drvcppchannel[drvccp_registered_chcount].pinnr = 5;
                break;
            default:
                break;
        }
        DrvCPPInitChannel(chnr);
        DrvGpioClearPin(drvcppchannel[drvccp_registered_chcount].port, drvcppchannel[drvccp_registered_chcount].pinnr);
        drvccp_registered_chcount++;
    return drvccp_registered_chcount - 1;
    }
    else
    {
        return 0xFF;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvCCPUpdate(DRVCCP_CH_HNDL hndl, unsigned int period)
{
	CCP_REG* ccpreg;
    
    drvcppchannel[hndl].period = period;
    
    ccpreg = &drvcppreg[drvcppchannel[hndl].chnr];
    *ccpreg->ccp_period_l = (unsigned char)(period & 0x00FF);
    *ccpreg->ccp_period_h = (unsigned char)((period >> 8) & 0x00FF);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvCCPReset(DRVCCP_CH_HNDL hndl)
{
    CCP_REG* ccpreg;
    
    drvcppchannel[hndl].period = 0;
    drvcppchannel[hndl].state = 0;
    ccpreg = &drvcppreg[drvcppchannel[hndl].chnr];
    
    *ccpreg->ccp_ctrl &= 0xF0;
    DrvGpioClearPin(drvcppchannel[hndl].port, drvcppchannel[hndl].pinnr);
    *ccpreg->ccp_period_h &= 0x00;
    *ccpreg->ccp_period_l &= 0x00;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvCCPStartTimer(DRVCCP_CH_HNDL hndl)
{
    CCP_REG* ccpreg;
    ccpreg = &drvcppreg[drvcppchannel[hndl].chnr];
    *ccpreg->ccp_tmr_ctrl &= 0xFE; //Disable Timer
    TMR3H = 0;
    TMR3L = 0;
    *ccpreg->ccp_tmr_ctrl |= 0x01;  //Start Timer 
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvCCPStart(DRVCCP_CH_HNDL hndl, CCP_POL pol)
{
    CCP_REG* ccpreg;
    drvcppchannel[hndl].pol = pol;
    ccpreg = &drvcppreg[drvcppchannel[hndl].chnr];
 
    if(drvcppchannel[hndl].pol == CCP_ACTIVE_H)    
    {
        DrvGpioClearPin(drvcppchannel[hndl].port, drvcppchannel[hndl].pinnr);
        *ccpreg->ccp_ctrl = 0x08;        //force CCP-pin high on compare match   
    }
    else if(drvcppchannel[hndl].pol == CCP_ACTIVE_L)    
    {
        *ccpreg->ccp_ctrl = 0x09;        //force CCP-pin low on compare match
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvCCPStop(DRVCCP_CH_HNDL hndl)
{
    CCP_REG* ccpreg;
    
    ccpreg = &drvcppreg[drvcppchannel[hndl].chnr];
    //*ccpreg->ccp_ctrl &= 0xF0; 
    DrvGpioClearPin(drvcppchannel[hndl].port, drvcppchannel[hndl].pinnr);
    if(drvcppchannel[hndl].pol == CCP_ACTIVE_H)         
    {
        //DrvGpioSetPin(drvcppchannel[hndl].port, drvcppchannel[hndl].pinnr);
    }
    else if(drvcppchannel[hndl].pol == CCP_ACTIVE_L)    
    {
        //DrvGpioClearPin(drvcppchannel[hndl].port, drvcppchannel[hndl].pinnr);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************/
