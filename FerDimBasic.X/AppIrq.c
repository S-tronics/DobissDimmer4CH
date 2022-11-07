/*
 * File:   AppIrq.c
 * Author: Stijn Vermeersch
 *
 * Created on 09/09/2021
 */

/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "..\Source\System\PIC18F66K80\SysLibAll.h"

//Driver Include Section
#include "..\Source\Driver\PIC18F66K80\DrvTimerTick.h"
#include "..\Source\Driver\PIC18F66K80\DrvTimer.h"
#include "..\Source\Driver\PIC18F66K80\DrvCan.h"
//Standard Include Section
#include "..\Source\Standard\Timer\StdTask.h" 

//Application Include Section
#include "AppIrq.h"
#include "AppZCD.h"
/**********************************************************************************************************************/

/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define INT_INTGIE  0x80        //Interrupt Global Enable
#define INT_PEIE    0x40        //Interrupt Periphery Enable



#define INT_CCP2IE  0x04        //Interrupt Capture Compare 2
#define INT_CCP2IF  0x04        //Intterupt Flag Capture Compare 2
#define INT_CCP3IE  0x01        //Interrupt Capture Compare 3
#define INT_CCP3IF  0x01        //Interrupt Flag Capture Compare 3
#define INT_CCP4IE  0x02        //Interrupt Capture Compare 4
#define INT_CCP4IF  0x02        //Interrupt Flag Capture Compare 4
#define INT_CCP5IE  0x04        //Interrupt Capture Compare 5 
#define INT_CCP5IF  0x04        //Interrupt Flag Capture Compare 5

#define INT_TMR2IE  0x02        //Timer 2 Interrupt Enable
#define INT_TMR2IP  0x02        //Timer 2 interrupt priority
#define INT_TMR2IF  0x02        //Timer 2 Interrupt Flag

#define INT_TMR4IE  0x80        //Timer 4 Interrupt Enable
#define INT_TMR4IP  0x80        //Timer 4 Interrupt Priority
#define INT_TMR4IF  0x80        //Timer 4 Interrupt Flag

#define INT_TX1IE   0x10        //Uart1 TX Interrupt Enable
#define INT_TX1IF   0x10        //Uart1 TX Interrupt Flag
#define INT_RX1IE   0x20        //Uart1 RX Interrupt Enable
#define INT_RX1IF   0x20        //Uart1 RX Interrupt Flag

#define INT_CAN_RXB0IE	0x01	//CAN RX Buffer 0 Interrupt Enable
#define INT_CAN_RXB0IF	0x01	//CAN RX Buffer 0 Interrupt Flag
#define INT_CAN_RXB0IP	0x01	//CAN RX Buffer 0 Interrupt Priority
#define INT_CAN_RXB1IE	0x02	//CAN RX Buffer 1 Interrupt Enable
#define INT_CAN_RXB1IF	0x02	//CAN RX Buffer 1 Interrupt Flag
#define INT_CAN_RXB1IP	0x02	//CAN RX Buffer 1 Interrupt Priority




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
static unsigned int ctr = 0;
/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppIrqInit(void)
{
    //INTCON3 &= ~INT_INT1IF;         //Clear External Interrupt INT1 Flag
    INTCON3bits.INT1IF = 0;         //Clear External Interrupt INT1 Flag
    //PIR3 &= ~INT_CCP2IF;            //Clear Capture Compare 2 Interrupt Flag
    PIR3bits.CCP2IF = 0;            //Clear Capture Compare 2 Interrupt Flag
    //PIR4 &= ~INT_CCP3IF;            //Clear Capture Compare 3 Interrupt Flag
    PIR4bits.CCP3IF = 0;            //Clear Capture Compare 3 Interrupt Flag
    //PIR4 &= ~INT_CCP4IF;            //Clear Capture Compare 4 Interrupt Flag
    PIR4bits.CCP4IF = 0;            //Clear Capture Compare 4 Interrupt Flag  
    //PIR4 &= ~INT_CCP5IF;            //Clear Capture Compare 5 Interrupt Flag
    PIR4bits.CCP5IF = 0;            //Clear Capture Compare 5 Interrupt Flag
    
    //Enable Global Interrupt & Interrupt priorities
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;            //Enables Global Interrupt Enable High Interrupts
    INTCONbits.GIEL = 1;            //Enables Global Interrupt Enable Low Interrupts
    //INTCON |= INT_INTGIE;
    //INTCON |= INT_PEIE;
    //Enable External INT1 to catch Interrupt ZCD (Zero Cross Detection)   
    INTCON3bits.INT1E = 1;          //Enable external interrupt 1 (INT1) 
    INTCON3bits.INT1IP = 1;         //Enable high priority for INT1.
    INTCON2bits.INTEDG1 = 1;        //INT1 occurs on rising edge
    //INTCON3 |= INT_INT1IE;
    //INTCON2 |= INT_INTEDG1;         //Edge of INT1
    
    //Enable Capture Compare Interrupt 
    PIE3bits.CCP2IE = 1;            //Interrupt enable for CCP2
    IPR3bits.CCP2IP = 1;            //Enable high priority for CCP2
    PIE4bits.CCP3IE = 1;            //Interrupt enable for CCP3 
    IPR4bits.CCP3IP = 1;            //Enable high priority for CCP3
    PIE4bits.CCP4IE = 1;            //Interrupt enable for CCP4
    IPR4bits.CCP4IP = 1;            //Enable high priority for CCP4
    PIE4bits.CCP5IE = 1;            //Interrupt enable for CCP5
    IPR4bits.CCP5IP = 1;            //Enable high priority for CCP5
//    PIE3 |= INT_CCP2IE;             //CCP2
//    PIE4 |= INT_CCP3IE;             //CCP3
//    PIE4 |= INT_CCP4IE;             //CCP4
//    PIE4 |= INT_CCP5IE;             //CCP5
    //Timer2 Interrupt enable       //For Task Handler.
    PIE1bits.TMR2IE = 1;            //Interrupt enable for TMR2
    PIR1bits.TMR2IF = 0;            //Clear interrupt flag for TMR2
    IPR1bits.TMR2IP = 0;            //Interrupt priority low for TMR2
    //PIE1 |= INT_TMR2IE; 
    //Timer4 Interrupt Enable       //For flying wheel
    PIE4bits.TMR4IE = 1;            //Interrupt enable for TMR4        
    PIR4bits.TMR4IF = 0;            //Clear interrupt flag for TMR4
    IPR4bits.TMR4IP = 1;            //Interrupt priority low for TMR4
    //PIE4 |= INT_TMR4IE;     
    //Serial Communication UART1 Receive Interrupt Enable
    PIE1bits.RC1IE = 1;
    PIR1bits.RC1IF = 0;
    //PIE1 |= INT_RX1IE;
    
	
    /*---  For Mode 0  ---*/
//    //CAN-bus Receive buffer 0
//	PIE5bits.RXB0IE = 1;				//CAN RX-buffer 0 Interrrupt Enable
//	PIR5bits.RXB0IF = 0;				//Clear interrupt flag for CAN RX-buffer 0
//	IPR5bits.RXB0IP = 0;				//Interrupt priority for CAN RX-buffer 0
//	//CAN-bus Receive buffer 1
//	PIE5bits.RXB1IE = 1;				//CAN RX-buffer 1 Interrupt Enable
//	PIR5bits.RXB1IF = 0;				//Clear interrupt flag for CAN RX-buffer 1
//	IPR5bits.RXB1IP = 0;				//Interrupt priority for CAN RX-buffer 1
    
    /*---  For Mode 1 or 2  ---*/
	//CAN-bus Receive buffer 0
	PIE5bits.RXBnIE = 1;				//CAN RX-buffer 0 Interrrupt Enable
	PIR5bits.RXBnIF = 0;				//Clear interrupt flag for CAN RX-buffer n
	IPR5bits.RXBnIP = 0;				//Interrupt priority for CAN RX-buffer n
 
    //INTCON &= ~INT_INT0IF;      //Clear interrupt flag
    //PIR1 &= ~INT_TMR2IF;        //Clear Timer 2 Interrupt flag
    //PIR1 &= ~INT_RX1IF;         //Clear Uart1 Rx Interrupt flag
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*****************************************************************************************************
Functie "ScanInput"
 * Check for USB communication and give some time to interrupt (Make use of Usart2 interrupt)
 * Check for CAN interrupt
/*****************************************************************************************************/
/*void ScanInput(void)
{
    INTCONbits.GIEL = 0;                        // Disable LOW Interrupt bit
    //module.intBlock = 1;

    AppCanReceive();
    if (PIR5bits.ERRIF == 1)                  // CAN Error INT
    {
        // Recover from scenario where CAN no longer receives anything
        RXB0CONbits.RXFUL = 0;
        COMSTATbits.RXB0OVFL = 0;
        RXB1CONbits.RXFUL = 0;
        COMSTATbits.RXB1OVFL = 0;
        PIR5bits.ERRIF = 0;
    }
    if (PIR1bits.TMR1IF == 1)                    // Timer1 interrupt - RTC
    {
        PIR1bits.TMR1IF = 0;
        //WriteTimer1(0x8000);                      // exact 1 sec.
        //AppRTCSeconds();                          // duration: 213µs
    }
    
    INTCONbits.GIEL = 1;                         // Enable LOW Interrupt bit
    //module.intBlock = 0;
}*/
/*--------------------------------------------------------------------------------------------------------------------*/
void AppIrqIntDisable(void)
{
    //INTCON &= ~INT_INTGIE;
    INTCON &= ~INT_PEIE;            //Disables all low-priority interrupts
    INTCON &= ~INT_INTGIE;          //Disables all interrupts
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppIrqIntEnable(void)
{
    //INTCON |= INT_INTGIE;
    INTCON |= INT_INTGIE;           //Enables all high-priority peripheral interrupts
    INTCON |= INT_PEIE;             //Enables all low-priority peripheral interrupts
}
/*--------------------------------------------------------------------------------------------------------------------*/
void __interrupt(high_priority) AppIrqISRHigh(void)
{
    if(INTCON3bits.INT1F == 1) {
        AppZCDFlywheel();            //External Interrupt (INT1) Activated
        INTCON2bits.INTEDG1 = !INTCON2bits.INTEDG1; 
        INTCON3bits.INT1IF = 0;
    }
    else{
        if(PIR4bits.TMR4IF == 1){          //Timer for zcd_period definition
            drvtmr_tick_count++;
            PIR4bits.TMR4IF = 0;
        }
        if(PIR3bits.CCP2IF == 1){
            AppZCDChannel(0);
            PIR3bits.CCP2IF = 0;
        }
        if(PIR4bits.CCP3IF == 1){
            AppZCDChannel(1);
            PIR4bits.CCP3IF = 0;
        }
        if(PIR4bits.CCP4IF == 1){
            AppZCDChannel(2);
            PIR4bits.CCP4IF = 0;
        }
        if(PIR4bits.CCP5IF == 1){
            AppZCDChannel(3);
            PIR4bits.CCP5IF = 0;
        }
        
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void __interrupt(low_priority) AppIrqISRLow(void)
{
    if(PIR1bits.TMR2IF == 1)
	{               //Timer for scheduler
        drvtmrtick_tick_count++;
        StdTaskTick();
        PIR1bits.TMR2IF = 0;
    }
    else if(PIR1bits.RC1IF == 1)
	{
        PIR1bits.RC1IF = 0;
    }
    //For Mode0
//	else if((PIR5bits.RXB0IF == 1) || (PIR5bits.RXB1IF == 1))		//CAN message received.
//	{
//		DrvCanRxISR();
//	}
    //For Mode1
    else if(PIR5bits.RXBnIF == 1)
	{
		DrvCanRxISR();
	}
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*void __interrupt() AppIrqISR(void)
{
    //Everything arround interrupts
    if(INTCON3 & INT_INT1IF)            //External Interrupt (INT1) Activated
    {
        AppZCDFlywheel();
        
    }
    else if (PIR3 & INT_CCP2IF)         //Capture Compare 2 Interrupt Activated
    {
        AppZCDChannel(0);
        PIR3 &= ~INT_CCP2IF;            //Clear Capture Compare 2 Interrupt Flag
    }
    else if (PIR4 & INT_CCP3IF)         //Capture Compare 3 Interrupt Activated
    {
        AppZCDChannel(1);
        PIR4 &= ~INT_CCP3IF;            //Clear Capture Compare 3 Interrupt Flag
    }
    else if (PIR4 & INT_CCP4IF)         //Capture Compare 4 Interrupt Activated
    {
        AppZCDChannel(2);
        PIR4 &= ~INT_CCP4IF;            //Clear Capture Compare 4 Interrupt Flag
    }
    else if (PIR4 & INT_CCP5IF)         //Capture Compare 5 Interrupt Activated
    {
        AppZCDChannel(3);
        PIR4 &= ~INT_CCP5IF;            //Clear Capture Compare 5 Interrupt Flag
    }
    else if (PIR1 & INT_TMR2IF)         //Timer2 Interrupt
    {
        drvtmrtick_tick_count++;
        StdTaskTick();
        PIR1 &= ~INT_TMR2IF;            //Clear interrupt flag
    }
    else if (PIR4 & INT_TMR4IF)         //Timer 4 Interrupt  (Check if ZCD pulse is in the wright period)
    {
        drvtmr_tick_count++;
        PIR4 &= ~INT_TMR4IF;
    }
    else if (PIR1 & INT_RX1IF)
    {
        PIR1 &= ~INT_RX1IF;             //Clear Uart1 Rx Interrupt flag
    }
}
*/
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/


