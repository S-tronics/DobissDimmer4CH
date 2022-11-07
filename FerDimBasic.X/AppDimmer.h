/**********************************************************************************************************************/
/**
 * @file        AppDimmer.h
 *
 * @author      Stijn Vermeersch
 * @date        26.07.2021
 *
 * @brief      
 *
 * \n<hr>\n
 * Copyright (c) 2021, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#ifndef APPDIMMER_H
#define	APPDIMMER_H
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
//DRIVER Include section
#include "..\Source\Driver\PIC18F66K80\DrvGpio.h"
//APPLICATION Include section
#include "AppLed.h"

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S Y M B O L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define NO          0
#define UP          1
#define DOWN        2  

#define MODE        PORTEbits.RE1
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
typedef union {
    unsigned char all;
    struct {
        unsigned delay:7;
        unsigned type:1;        // 0=sec, 1=min
    };
}
DELAY;

typedef struct {
    unsigned int location;
    unsigned char colMaxCount;
    unsigned int rowCount;
    unsigned char colDataCount;
}
LOC;

typedef union {
    unsigned char all;
    struct {
        unsigned flick:1;       // flikkeren van output
        unsigned follow:1;      // pulsvolger
        unsigned dim:1;         // dimmen via BUS
        unsigned res:5;         // reserved
    };
}
PURP;

//extern PURP purp;
PURP purp;

typedef union {
    unsigned char all[16];
    struct {
        unsigned char address;
        unsigned char channel;
        unsigned char actionid;
        DELAY delayOn;
        DELAY delayOff;
        unsigned char value;
        unsigned char softDim;
        unsigned char cond;
        unsigned char res[8];
    };
}
ACTION; 
ACTION action;

typedef struct {
    unsigned scan:1;                    // [0x10C]
    unsigned dallasNr:1;                // [0x10C]
    unsigned char shift;                // [0x10D]
 }
INTER;
INTER inter;

typedef union
{
	unsigned char all[24];
	struct
	{
		unsigned char address;			// EE (0x00)
		unsigned char time2function;    // EE (0x01)  (1=0,5sec - 2=1sec - 3=1,5sec - .... - 10=5sec)
		unsigned char master;           // EE (0x02)
		unsigned char res[11];          // EE (0x03 - 0x0D)
		unsigned char type;             // EE (0x0E)
		unsigned char version;          // EE (0x0F)
		
		unsigned char timeout:1;               // module.all[16] on startup = 0
		unsigned char dallasButtonScan:1;      // module.all[16] on startup = 0
		unsigned char service:1;               // module.all[16] on startup = 0 --> used for detection when in Prog-mode
		unsigned char res1:5;                  // Reserved bits

		unsigned char toolScan:1;              // module.all[17] on startup = 0 --> Tool programmer in use
		unsigned char toolWait:1;              // module.all[17] on startup = 0 --> Tool programmer in use
		unsigned char buttonScan:1;            // module.all[17] on startup = 0 --> Button scan in use
		unsigned char intBlock:1;              // module.all[17] on startup = 0 --> when 1 no interrupt handling
		unsigned char int0int:1;
		unsigned char buttonPushed:1;          // module.all[17] on startup = 0 --> 1=analog button pushed, 0=not
		unsigned char res2:2;				  // Reserved bits	
      
		unsigned char toolTimer;          // module.all[19]                --> Tool programmer. Timer to break Tool Configuration setting after max. 240 sec.

		unsigned char res4[4];         	  // Reserved
	};
}
MODULE;
MODULE module;

typedef union {
    unsigned char all[83];
    struct {
        // 64 bytes from EEPROM
        ACTION onAction;
        ACTION offAction;
        DELAY pulse;
        unsigned char lock1;
        unsigned char lock2;
        unsigned char res[13];  
        //unsigned char loadType;
        unsigned char ph;
        unsigned char tStart;
        unsigned char tUp1;
        unsigned char tUp0;
        unsigned char tJump;
        unsigned char tDown1;
        unsigned char tDown0;
        unsigned char vJump;
        unsigned char vStart;
        unsigned char vMin;
        unsigned char runSpeed;
        unsigned char vMax;        
        unsigned char syncOutput;
        unsigned char res2[3];
        
        // other in RAM; Stored in RAM after reading out from eeprom
        unsigned char ch;
        bool en;
        unsigned char actValue;         
        unsigned int tUp;
        unsigned int tUpdef;
        unsigned int tDown;
        unsigned int tDowndef;
        unsigned char vJumpValue;
        unsigned char moving;
        unsigned int speed;
        unsigned char starting;
        unsigned char endValue;
        unsigned int realValue;
        unsigned char step;
        unsigned char direction;
        bool running;
        APPLED led;
        GPIO_PORT port_reset;
        unsigned char pin_reset;
        GPIO_PORT port_overtemperature;
        unsigned char pin_overtemperature;
        GPIO_PORT port_overcurrent;
        unsigned char pin_overcurrent;
        bool overtemperature;
        bool overcurrent;
    };
}
DIM_CH;
DIM_CH dim_ch[4];

typedef union {
    unsigned char all[17];
    struct {
        ACTION action;
        PURP Purp;
    };
}CMD;
CMD cmd[5];                 

typedef union {
    unsigned char all[5];
    struct {
        unsigned char sec;
        unsigned int min;
        unsigned char flicksec;
        unsigned char oldstatus;
    };
}
RTCCOUNT;
RTCCOUNT rtccount[4];

unsigned char DIMrunning[3][4];
unsigned char RTCminutes;
//CMD CMD_pulsvolger;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Function \n
 *
 * Description \n
 * 
 */
void AppDimmerInit(void);

/**
 * @brief   Function \n
 *
 * Description \n
 * 
 */
void AppDimmerReset(void);

/**
 * @brief   Function \n
 *
 * Description \n
 * 
 */
void AppDimmerControl(void);

/**
 * @brief   Function \n
 *
 * Description \n
 * 
 */
void AppDimmerUpdate(void);

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/
#endif	/* APPLED_H */
