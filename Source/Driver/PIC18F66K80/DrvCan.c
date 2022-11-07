/**********************************************************************************************************************/
/**
 * @file        can\DrvCan.c
 *
 * @author      Stijn Vermeersch
 * @date        27.07.2021
 *
 * @brief       Basic CAN-controller functionality
 *
 * The CAN-controller is used to maintain CAN-communication
 *
 * \n<hr>\n
 * Copyright (c) 2021, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#define CAN__DRVCAN_C
/**********************************************************************************************************************/



/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/
//#include "AppConfig.h"

/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include <stdbool.h>
#include "..\..\System\PIC18F66K80\SysLibAll.h"

//DRIVER lib include section
//#include "ecan\ECAN.h" 
#include "DrvGpio.h"
#include "DrvCan.h"
//APPLICATION lib include section
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define DRVCAN_RXM0_VAL     0x01FCFF00
#define DRVCAN_RXM1_VAL     0x01FCFF00

#define DRVCAN_RX_QUEUE_LENGTH  60
#define DRVCAN_TX_QUEUE_LENGTH  3
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
typedef struct 
{
    volatile unsigned char* cancon;             //can bus configuration register
    volatile unsigned char* canstat;            //can status register
    volatile unsigned char* rxb0con;            //receive buffer 0 control register
    volatile unsigned char* rxb1con;            //receive buffer 1 control register
    volatile unsigned char* b0con;              //receive buffer 0 control register (mode 1)
    volatile unsigned char* b1con;              //receive buffer 1 control register (mode 1))
    volatile unsigned char* bsel;               //Buffer selection register
    volatile unsigned char* rxb0sidh;           //Receive id buffer 0
    volatile unsigned char* rxb1sidh;           //Receive id buffer 1
    volatile unsigned char* rxb0d0;             //Receive buffer 0, databyte 0;
    volatile unsigned char* rxb1d0;             //Receive buffer 1, databyte 0;
    volatile unsigned char* rxb0dlc;            //dlc of rx-buffer 0
    volatile unsigned char* rxb1dlc;            //dlc of rx-buffer 1
    volatile unsigned char* brgcon1;            //baudrate generator control register 1
    volatile unsigned char* brgcon2;            //baudrate generator control register 2
    volatile unsigned char* brgcon3;            //baudrate generator control register 3
    volatile unsigned char* ciocon;         
    volatile unsigned char* ecancon;            //Enhanced can control register   
    volatile unsigned char* rxm0sidl;
    volatile unsigned char* rxm1sidl;
    volatile unsigned char* rxf0sidl;
    volatile unsigned char* rxf1sidl;
    volatile unsigned char* rxf2sidl;
    volatile unsigned char* rxf3sidl;
    volatile unsigned char* rxf4sidl;
    volatile unsigned char* rxf5sidl;
    volatile unsigned char* rxf6sidl;
    volatile unsigned char* rxf7sidl;
    volatile unsigned char* rxf8sidl;
    volatile unsigned char* rxf9sidl;
    volatile unsigned char* rxf10sidl;
    volatile unsigned char* rxf11sidl;
    volatile unsigned char* rxf12sidl;
    volatile unsigned char* rxf13sidl;
    volatile unsigned char* rxf14sidl;
    volatile unsigned char* rxf15sidl;
    volatile unsigned char* rxfbcon0;           //Receive filter buffer control register 0 (Filter 0 en Filter 1)
    volatile unsigned char* rxfbcon1;           //Receive filter buffer control register 1 (Filter 2 en Filter 3)
    volatile unsigned char* rxfbcon2;           //Receive filter buffer control register 2 (Filter 4 en Filter 5)
    volatile unsigned char* rxfbcon3;           //Receive filter buffer control register 3 (Filter 6 en Filter 7)
    volatile unsigned char* rxfbcon4;           //Receive filter buffer control register 4 (Filter 8 en Filter 9)
    volatile unsigned char* rxfbcon5;           //Receive filter buffer control register 5 (Filter 10 en Filter 11)
    volatile unsigned char* rxfbcon6;           //Receive filter buffer control register 6 (Filter 12 en Filter 13)
    volatile unsigned char* rxfbcon7;           //Receive filter buffer control register 7 (Filter 14 en Filter 15)
    volatile unsigned char* rxfcon0;            //Receive filter control register 0
    volatile unsigned char* rxfcon1;            //Receive filter control register 1
    volatile unsigned char* txb0con;            //Transmit buffer0 control register
    volatile unsigned char* txb1con;            //Transmit buffer1 control register
    volatile unsigned char* txb2con;            //Transmit buffer2 control register    
    volatile unsigned char* txb0sidh;           //Transmit id buffer 0; baseaddress (used for extended id also)
    volatile unsigned char* txb1sidh;           //Transmit id buffer 1; baseaddress (used for extended id also)
    volatile unsigned char* txb2sidh;           //Transmit id buffer 2; baseaddress (used for extended id also)
    volatile unsigned char* txb0d0;             //Transmit buffer 0, databyte 0;
    volatile unsigned char* txb1d0;             //Transmit buffer 1, databyte 0;
    volatile unsigned char* txb2d0;             //Transmit buffer 2, databyte 0;
    volatile unsigned char* txb0dlc;            //dlc of tx-buffer 0
    volatile unsigned char* txb1dlc;            //dlc of tx-buffer 1   
    volatile unsigned char* txb2dlc;            //dlc of tx-buffer 2

    volatile unsigned char* perint_en;          //Peripheral interrupt enable
    volatile unsigned char* bufint;             //Buffer interrupt enable

    /*RX Hook, code that executes when a CAN-message is received*/
    DRVCAN_RXHOOK drvcan_rx;                    //Hook will be executed when a CAN-message is received.
}
CAN_BUS;

typedef enum _CAN_OP_MODE
{
    ECAN_OP_MODE_BITS    = 0xe0,   // Use this to access opmode bits
    ECAN_OP_MODE_NORMAL  = 0x00,
    ECAN_OP_MODE_SLEEP   = 0x20,
    ECAN_OP_MODE_LOOP    = 0x40,
    ECAN_OP_MODE_LISTEN  = 0x60,
    ECAN_OP_MODE_CONFIG  = 0x80
}
CAN_OP_MODE;

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvCanIDtoReg(unsigned char* ptr, unsigned long val);          //Always use extended bits
void DrvCanRegtoCanID(unsigned char* ptr,  unsigned long *id);               //Always use extended bits
void DrvCanSetOperationalMode(CAN_OP_MODE mode);
void DrvCanInitReceiveQueue(void);
bool DrvCanFillReceiveQueue(CAN_MESSAGE* msg);
bool DrvCanReadReceiveQueue(CAN_MESSAGE* msg);
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static CAN_BUS          drvcan;
static unsigned char    drvcanerror;                                //bit0 = txbuffer overflow; bit5 = rx queue overflow
static unsigned char    drvcanrxhead;
static unsigned char    drvcanrxtail;
static bool             drvcanrxfull = false;
static CAN_MESSAGE      rxmsg;
static CAN_MESSAGE      drvcan_rxdata[DRVCAN_RX_QUEUE_LENGTH];      //CAN message receive queue

/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvCanIDtoReg(unsigned char* ptr, unsigned long val)
{
    CAN_MESSAGE_ID* id;

    id = (CAN_MESSAGE_ID*)&val;
    *ptr = id->BYTES.BYTE3 >> 5;
    *ptr |= id->BYTES.BYTE4 << (unsigned char)3;
    ptr++;
    *ptr = (id->BYTES.BYTE3 << 3) & 0xE0;
    *ptr |= 0x08;                //Enable extended ID-bit
    *ptr |= id->BYTES.BYTE3 & 0x03;
    ptr++;
    *ptr = id->BYTES.BYTE2;
    ptr++;
    *ptr = id->BYTES.BYTE1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvCanRegtoCanID(unsigned char* ptr,  unsigned long *id)
{
    CAN_MESSAGE_ID *ID;

    ID = (CAN_MESSAGE_ID*)id;
    // Extended Identifier
    ID->BYTES.BYTE3 = (*ptr << 5);              // Copy SIDH<2:0> to EID<23:21>
    ID->BYTES.BYTE4 = (*ptr >> 3);              // Copy SIDH<7:3> to EID<29:25>
    ptr++;                                      // Point to SIDL
    ID->BYTES.BYTE3 |= (*ptr & 0x03);           // Copy SIDH<1:0> to EID<17:16>
    ID->BYTES.BYTE3 |= ((*ptr & 0xE0) >> 3);    // Copy SIDL<7:6> to EID<20:18>
    ptr++;                                      // Point to EIDH
    ID->BYTES.BYTE2 = *ptr;                     // Copy EIDH<15:8> to EID<15:8>
    ptr++;                                      // Point to EIDL
    ID->BYTES.BYTE1 = *ptr;                     // Copy EIDH<7:0> to EID<7:0>
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvCanSetOperationalMode(CAN_OP_MODE mode)
{
    *drvcan.cancon &= 0x1F;
    *drvcan.cancon |= mode;
    while((*drvcan.cancon & 0xE0) != mode); // Wait till desired mode is set.
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvCanInitReceiveQueue(void)
{
    drvcanrxhead = 0;
    drvcanrxtail = 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool DrvCanFillReceiveQueue(CAN_MESSAGE* msg)
{
    signed char diff = 0;
    if(drvcanrxfull == true)
    {
        drvcanerror |= 0x20;
        return false;
    }
    diff = drvcanrxhead - drvcanrxtail;
    if(diff < 0)    diff += DRVCAN_RX_QUEUE_LENGTH;
    if(diff >= DRVCAN_RX_QUEUE_LENGTH)   
    {   
        DrvGpioSetPin(PORT_D, 7);
        return false;
    }
    drvcan_rxdata[drvcanrxhead] = *msg;
    drvcanrxhead++;
    if(drvcanrxhead == DRVCAN_RX_QUEUE_LENGTH)  drvcanrxhead = 0;
    if(drvcanrxhead == drvcanrxtail)    drvcanrxfull = true;
    return true;
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool DrvCanReadReceiveQueue(CAN_MESSAGE* msg)
{
    if((drvcanrxhead == drvcanrxtail)&&(!drvcanrxfull))         //Receivequeue is empty
    {
        //drvcanerror |= 0x20;
        return false;
    }
    *msg = drvcan_rxdata[drvcanrxtail];
    drvcanrxtail++;
    if(drvcanrxtail == DRVCAN_RX_QUEUE_LENGTH)  drvcanrxtail = 0;
    return true;
}
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvCanInit(unsigned char address)
{
	unsigned long int filters[4];
// -----------------------------------------------------
// CAN BUS initialisatie + Configuration
// -----------------------------------------------------

    drvcan.cancon = &CANCON;
    drvcan.canstat = &CANSTAT;
    drvcan.rxb0con = &RXB0CON;
    drvcan.rxb1con = &RXB1CON;
    drvcan.b0con = &B0CON;
    drvcan.b1con = &B1CON;
    drvcan.bsel = &BSEL0;
    drvcan.rxb0sidh = &RXB0SIDH;
    drvcan.rxb1sidh = &RXB1SIDH;
    drvcan.rxb0d0 = &RXB0D0;
    drvcan.rxb1d0 = &RXB1D0;
    drvcan.rxb0dlc = &RXB0DLC;
    drvcan.rxb1dlc = &RXB1DLC;
    drvcan.brgcon1 = &BRGCON1;
    drvcan.brgcon2 = &BRGCON2;
    drvcan.brgcon3 = &BRGCON3;
    drvcan.ciocon = &CIOCON;
    drvcan.ecancon = &ECANCON;
    drvcan.rxfcon0 = &RXFCON0;
    drvcan.rxfcon1 = &RXFCON1;
    drvcan.rxm0sidl = &RXM0SIDL;
    drvcan.rxm1sidl = &RXM1SIDL;
    drvcan.rxf0sidl = &RXF0SIDL;
    drvcan.rxf1sidl = &RXF1SIDL;
    drvcan.rxf2sidl = &RXF2SIDL;
    drvcan.rxf3sidl = &RXF3SIDL;
    drvcan.rxf4sidl = &RXF4SIDL;
    drvcan.rxf5sidl = &RXF5SIDL;
    drvcan.rxf6sidl = &RXF6SIDL;
    drvcan.rxf7sidl = &RXF7SIDL;
    drvcan.rxf8sidl = &RXF8SIDL;
    drvcan.rxf9sidl = &RXF9SIDL;
    drvcan.rxf10sidl = &RXF10SIDL;
    drvcan.rxf11sidl = &RXF11SIDL;
    drvcan.rxf12sidl = &RXF12SIDL;
    drvcan.rxf13sidl = &RXF13SIDL;
    drvcan.rxf14sidl = &RXF14SIDL;
    drvcan.rxf15sidl = &RXF15SIDL;
    drvcan.rxfbcon0 = &RXFBCON0;
    drvcan.rxfbcon1 = &RXFBCON1;
    drvcan.rxfbcon2 = &RXFBCON2;
    drvcan.rxfbcon3 = &RXFBCON3;
    drvcan.rxfbcon4 = &RXFBCON4;
    drvcan.rxfbcon5 = &RXFBCON5;
    drvcan.rxfbcon6 = &RXFBCON6;
    drvcan.rxfbcon7 = &RXFBCON7;
    drvcan.txb0con  = &TXB0CON;
    drvcan.txb1con  = &TXB1CON;
    drvcan.txb2con  = &TXB2CON;
    drvcan.txb0sidh = &TXB0SIDH;
    drvcan.txb1sidh = &TXB1SIDH;
    drvcan.txb2sidh = &TXB2SIDH;
    drvcan.txb0d0 = &TXB0D0;
    drvcan.txb1d0 = &TXB1D0;
    drvcan.txb2d0 = &TXB2D0;
    drvcan.txb0dlc = &TXB0DLC;
    drvcan.txb1dlc = &TXB1DLC;
    drvcan.txb2dlc = &TXB2DLC;
    drvcan.perint_en = &PIE5;
    drvcan.bufint = &BIE0;

    drvcan.drvcan_rx = NULL;
    TRISBbits.TRISB3 = 1;
    
    *drvcan.cancon &= 0x1F;

    DrvCanSetOperationalMode(ECAN_OP_MODE_CONFIG);
    
    while((*drvcan.cancon & 0x80) != 0x80);                 //waiting for CAN configuration mode

    *drvcan.bsel &= 0x00;              //All buffers are receiving buffers
    //Set CAN-controller mode
    *drvcan.ecancon &= ~0xC0;          
    //*drvcan.ecancon |= 0x00;           //Mode is legacy mode (mode 0)
    *drvcan.ecancon |= 0x40;           //Mode is enhanced legacy mode (mode 1)

    *drvcan.brgcon1 |= 0x04;           //SJW = 1; Baud Rate Prescaler bits = 4; TQ = (2 * (BRP +1)) / Fosc ==> TQ = 0.5µs
    *drvcan.brgcon2 |= 0xB1;           //Phase Segment 2 Freely programmable; Phase segment1 bits = 7 * TQ; Propgation time select bits = 2*TQ)
    *drvcan.brgcon2 &= ~0x40;          //Bus sampled once at the sample point
    *drvcan.brgcon2 |= 0x01;           //PROPSEG = (1+PRSEG) * TQ
    *drvcan.brgcon2 |= (0x06 << 3);    //Phase Segment 1 Time = (1+SEG1PH) * TQ
    *drvcan.brgcon3 |= 0x05;           //Phase Segment 2 Time = (1+SEG2PH) * TQ
    
    *drvcan.ciocon &= ~0x40;           //Disable CAN TX2
    *drvcan.ciocon |= 0x20;            //Set this bit when using differential bus
    
    //*drvcan.rxb0con |= 0x40;           //RXM = 10 : Only messages with extended id on rx-buffer 0; 
    //*drvcan.rxb1con |= 0x40;           //RXM = 10 : Only messages with extended id on rx-buffer 1
    *drvcan.rxb0con &= ~0x40;               //RXM = 0; Only accept filtered messages
    *drvcan.rxb1con &= ~0x40;               //RXM = 0; Only accept filtered messages
    *drvcan.b0con &= ~0x40;                 //RXM = 0; Only accept filtered messages
    *drvcan.b1con &= ~0x40;                 //RXM = 0; Only accept filtered messages
    
    //Set mask value; Mode0
    DrvCanIDtoReg(&RXM0SIDH, DRVCAN_RXM0_VAL);
    DrvCanIDtoReg(&RXM1SIDH, DRVCAN_RXM1_VAL);

    // Set filter values
    filters[0] = 0x01FCFF00;
    filters[1] = 0x0040FF00;
    filters[2] = 0x01FC0000 | ((unsigned int)address<<8);       // Send to module with address
    filters[3] = 0x00400000 | ((unsigned int)address<<8);       // Send to module with address and type (Filter for RXB0
    
    //RXB0 - filter
    DrvCanIDtoReg(&RXF0SIDH, filters[0]);
    DrvCanIDtoReg(&RXF1SIDH, filters[1]);
    DrvCanIDtoReg(&RXF2SIDH, filters[2]);
    DrvCanIDtoReg(&RXF3SIDH, filters[3]);
    //RXB1 - filter
    DrvCanIDtoReg(&RXF4SIDH, filters[0]);
    DrvCanIDtoReg(&RXF5SIDH, filters[1]);
    DrvCanIDtoReg(&RXF6SIDH, filters[2]);
    DrvCanIDtoReg(&RXF7SIDH, filters[3]);
    //B0 - filter
    DrvCanIDtoReg(&RXF8SIDH, filters[0]);
    DrvCanIDtoReg(&RXF9SIDH, filters[1]);
    DrvCanIDtoReg(&RXF10SIDH, filters[2]);
    DrvCanIDtoReg(&RXF11SIDH, filters[3]);
    //B1 - filter
    DrvCanIDtoReg(&RXF12SIDH, filters[0]);
    DrvCanIDtoReg(&RXF13SIDH, filters[1]);
    DrvCanIDtoReg(&RXF14SIDH, filters[2]);
    DrvCanIDtoReg(&RXF15SIDH, filters[3]);
    
    //Only extend ID's allowed for masks
    *drvcan.rxm0sidl |= 0x08;
    *drvcan.rxm1sidl |= 0x08;
    
    //Define which filters were connected (All)
    *drvcan.rxfcon0 = 0xFF;     //Filter 0 - 7 connected and enabled
    *drvcan.rxfcon1 = 0xFF;     //Filter 8 - 15 connected and enabled
    //Filters for RXB0
    *drvcan.rxfbcon0 = 0x00;    //Filter 0 - 3 connected with RXB0
    *drvcan.rxfbcon1 = 0x00;        
    //Filters for RXB1
    *drvcan.rxfbcon2 = 0x11;    //Filter 4 - 7 connected with RXB1
    *drvcan.rxfbcon3 = 0x11;
    //Filters for B0
    *drvcan.rxfbcon4 = 0x22;    //Filter 8 - 11 connected with B0
    *drvcan.rxfbcon5 = 0x22;
    //Filters for B1
    *drvcan.rxfbcon6 = 0x33;    //Filter 12 - 15 connected with B1
    *drvcan.rxfbcon7 = 0x33;

    BIE0 = 0x0F;                //Interrupt enable for all buffers
    // Set CAN module into Normal mode
    DrvCanSetOperationalMode(ECAN_OP_MODE_NORMAL);
    
    //*drvcan->perint_en = 0x00;                              //Clear all interrupt enables
    
    DrvGpioInitPin(PORT_D, 7, PIN_IO_FUNC_OUTPUT_LOW);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvCanRegisterRxHandler(DRVCAN_RXHOOK drvcan_rxhook)
{
    if(drvcan.drvcan_rx == NULL)    drvcan.drvcan_rx = drvcan_rxhook;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvCanSend(CAN_MESSAGE* msg)
{
    unsigned char i = 0, j = 0;
    unsigned char* txbncon[3], *canmsgptr;

    txbncon[0] = drvcan.txb0con;
    txbncon[1] = drvcan.txb1con;
    txbncon[2] = drvcan.txb2con;
    
    for(i = 0; i < 3; i++)
    {
        canmsgptr = txbncon[i];

        if(!(*canmsgptr & 0x08))
        {
            *canmsgptr &= ~0x03;            //Clear priority bits, no priority is used for the moment.
            *(canmsgptr + 5) = msg->dlc;
            DrvCanIDtoReg(canmsgptr + 1, msg->id.id);
            canmsgptr += 6;
            for(j = 0; j < msg->dlc; j++)
            {
                *canmsgptr++ = msg->data[j];
            }
            canmsgptr = txbncon[i];
            *canmsgptr |= 0x08;             //Ask to send CAN-message.
            return;                         //Return because CAN-controller was ready to send a CAN-message
        }
    }
    drvcanerror |= 0x01;            //Set txbuffer error bit, there was no possibility to send a Can message.
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool DrvCanRead(CAN_MESSAGE* msg)
{
    if(DrvCanReadReceiveQueue(msg)) return true;
    else    return false;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvCanRxISR(void)
{
    unsigned char i = 0, j = 0;
    unsigned char *rxbncon[4], *canmsgptr;
    rxbncon[0] = drvcan.rxb0con;
    rxbncon[1] = drvcan.rxb1con;
    rxbncon[2] = drvcan.b0con;
    rxbncon[3] = drvcan.b1con;

    
    if(PIR5bits.ERRIF)
        {
            Nop();
            PIR5bits.ERRIF = 0;
            COMSTATbits.RXBNOVFL = 0;
            RXB0CONbits.RXFUL = 0;
            RXB1CONbits.RXFUL = 0;
            B0CONbits.B0RXFUL = 0;
            B1CONbits.B1RXFUL = 0;
        }
    PIE5bits.RXBnIE = 0;				//CAN RX-buffer 0 Interrrupt Enable
    for(i = 0; i < 4; i++)
    {
        canmsgptr = rxbncon[i];
        if(*canmsgptr & 0x80)               //Receive buffer contains a received message
        {
            //DrvGpioSetPin(PORT_D, 7);
            DrvCanRegtoCanID(canmsgptr + 1, &rxmsg.id.id);
            rxmsg.dlc = *(canmsgptr + 5);
            canmsgptr += 6;
            for(j = 0; j < rxmsg.dlc; j++)
            {
                rxmsg.data[j] = *canmsgptr++;
            }
            rxmsg.bufnr = i;
            if(!DrvCanFillReceiveQueue(&rxmsg))
            {
                DrvGpioSetPin(PORT_D, 7);
            }
            canmsgptr = rxbncon[i];
            *canmsgptr &= ~0x80;            //Clear RXFUL
            /*-- Mode 0 --*/
            //PIR5 &= ~(1 << i);              //Clear the corresponding interrupt flag
            /*-- Mode 1 --*/
            PIR5 &= ~(0x2);              //Clear the corresponding interrupt flag
            //DrvGpioClearPin(PORT_D, 7);
        }
    }

    if(drvcan.drvcan_rx != NULL)
    {
        drvcan.drvcan_rx();
    }
    PIE5bits.RXBnIE = 1;				//CAN RX-buffer 0 Interrrupt Enable
    
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/
