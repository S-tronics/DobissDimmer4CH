/*
 * File:   Appmain.c
 * Author: S-tronics
 *
 * Created on 31 maart 2021, 13:11
 */

/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include <string.h>
#include "..\Source\System\PIC18F66K80\SysLibAll.h"
//Include Driver Function Libraries
#include "..\Source\Driver\PIC18F66K80\DrvGpio.h"
#include "..\Source\Driver\PIC18F66K80\DrvTimerTick.h"
#include "..\Source\Driver\PIC18F66K80\DrvI2C.h"
#include "..\Source\Driver\PIC18F66K80\DrvCCP.h"
#include "..\Source\Driver\PIC18F66K80\DrvTimer.h"
#include "..\Source\Driver\PIC18F66K80\DrvCan.h"

//Include Standard Function Libraries
#include "..\Source\Standard\Timer\StdTask.h"
#include "..\Source\Standard\I2C\Std24LC512T.h"
#include "..\Source\Standard\I2C\StdDS2484.h"
#include "..\Source\Standard\I2C\StdDS2485.h"
//Include Application Function Library
#include "AppEeprom.h"
#include "AppCan.h"
#include "AppLed.h"
#include "AppDimmer.h"
#include "AppConfig.h"
#include "AppIrq.h"
#include "AppZCD.h"
#include "AppButton.h"
#include "AppService.h"
#include "AppScenarios.h"
#include "AppRTC.h"
#include "AppAction.h"
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
static void AppMainInitAllObjects(void);
static void AppMainHeartbeatTask(void);
static void AppMainBackGroundLoop(void);
/**********************************************************************************************************************/

// PIC18F66K80 Configuration Bit Settings
// PIC18F25K80 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = HIGH   // SOSC Power Selection and mode Configuration bits (High Power SOSC circuit selected)
#pragma config XINST = OFF      // Extended Instruction Set (Disabled)

// CONFIG1H
#pragma config FOSC = HS2       // Oscillator (HS oscillator (High power, 16 MHz - 25 MHz))
#pragma config PLLCFG = OFF     // PLL x4 Enable bit (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power Up Timer (Disabled)
#pragma config BOREN = OFF      // Brown Out Detect (Disabled in hardware, SBOREN disabled)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (1.8V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer (WDT disabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 1048576  // Watchdog Postscaler (1:1048576)

// CONFIG3H
#pragma config CANMX = PORTB    // ECAN Mux bit (ECAN TX and RX pins are located on RB2 and RB3, respectively)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR Enabled, RE3 Disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size (2K word Boot Block size)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protect 00800-01FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 02000-03FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 04000-05FFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 06000-07FFF (Disabled)

// CONFIG5H
#pragma config CPB = OFF        // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protect 00800-01FFF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 02000-03FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 04000-05FFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 06000-07FFF (Disabled)

// CONFIG6H
#pragma config WRTC = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB = OFF       // Table Write Protect Boot (Disabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protect 00800-01FFF (Disabled)
#pragma config EBTR1 = OFF      // Table Read Protect 02000-03FFF (Disabled)
#pragma config EBTR2 = OFF      // Table Read Protect 04000-05FFF (Disabled)
#pragma config EBTR3 = OFF      // Table Read Protect 06000-07FFF (Disabled)

// CONFIG7H
#pragma config EBTRB = OFF      // Table Read Protect Boot (Disabled)

/***********************************************************************************************************************
; E X P O R T E D   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
typedef enum
{
    STARTUP,
    CONFIG,        
    RUNNING
}
MAIN_SEQ;
/**********************************************************************************************************************/
/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

static MAIN_SEQ mainstate = STARTUP;
static TASK_HNDL heartbeat;
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
static void AppMainInitAllObjects()
{
    static CAN_MESSAGE msg;
    
    static unsigned char statereg = 0x00;
    DrvGpioInit();
    DrvI2CInit();
    DrvCCPInit();

    DrvTimerTickInit(100);
    DrvTimerInit(100);  
    StdTaskInit(100);
    heartbeat =  StdTaskRegisterTask(1000000, (TASK_CALLBACK_FUNC)AppMainHeartbeatTask, NULL, 130);             //1 seconds timer (by interrupt)
    StdTaskStart(heartbeat);
    Std24LC512TInit();
    StdDS2484Init();
    StdDS2484DeviceReset();

    AppEepromInit();
    AppServiceInit();    
    AppZCDInit();
    AppLEDInit();
    AppButtonInit();
    AppDimmerInit();
    AppIrqInit();
    AppCanInit();
}

/**********************************************************************************************************************/
static void AppMainBackGroundLoop(void)
{
    if(AppServiceGet())             //Service button active?
    {
        AppButtonProg();
    }
    
    if(inter.scan)
    {
        AppButtonOneWireReset();
        AppCanHandler();
        inter.scan = 0;
    }
    
    StdTaskHandler();
//    AppScenarioHandler();
    AppActionHandler();
    AppZCDHandler();
}
/**********************************************************************************************************************/
static void AppMainHeartbeatTask(void)
{
    //DrvGpioTogglePin(PORT_A, 2);
    AppRTCSeconds();
}
/**********************************************************************************************************************/

/**********************************************************************************************************************/
void main(void) {
    
    AppMainInitAllObjects();
    
    for(;;)
    {
        AppMainBackGroundLoop();
    }
    return;
}
/**********************************************************************************************************************/