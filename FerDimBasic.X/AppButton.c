/*
 * File:   AppButton.c
 * Author: stronics
 *
 * Created on 27 July 2021, 13:11
 */

/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include <stdbool.h>
#include "AppConfig.h"
#include "..\Source\System\PIC18F66K80\SysLibAll.h"
//Driver Include Section
#include "..\Source\Driver\PIC18F66K80\DrvCan.h"
//Standard Include Section
#include "..\Source\Standard\Timer\StdTask.h"
#include "..\Source\Standard\I2C\StdDS2484.h"
#include "..\Source\Standard\I2C\StdDS2485.h"
#include "..\Source\Standard\I2C\Std24LC512T.h"
//Application Include Section
#include "AppIrq.h"
#include "AppButton.h"
#include "AppCan.h"
#include "AppScenarios.h"
#include "AppSfeer.h"
#include "AppAction.h"
#include "AppLed.h"
#include "AppDimmer.h"
#include "AppService.h"
/**********************************************************************************************************************/

/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define		SEARCH_ROM			0xF0
#define		PRESENCE_DETECT		0x02
#define     DEVICE_RESET        0x10      

#define     FILTER_ID_BUTTON    0x01
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   T Y P E D E F S  &  E N U M S
;---------------------------------------------------------------------------------------------------------------------*/
enum {ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8, IDMemHigh, IDMemLow, IDTimer, IDAction};
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
unsigned char AppButtonSearchRomOld(OWFILTER filter, bool usefilter);
//unsigned char AppButtonSearchRom(OWFILTER filter, bool usefilter);
unsigned char AppOWFirst(void);
unsigned char AppOWNext(void);
unsigned char AppSearchRom(void);

unsigned char checkCRC(unsigned char *id);
unsigned char findInProcessedButtons(unsigned char *id);
void AppButtonProcessPressed(unsigned char *id);
void AppButtonCheckPressedAction(unsigned char *id);
void AppButtonExecuteAction(unsigned char *id, unsigned char release);
void AppButtonTimerTask(void);
void AppButtonMsTask(void);
unsigned char AppButtonFindInFound(unsigned char *id);
unsigned char testLongPush(void);
/**
* @brief 	Function to search the Dallas ID in EEPROM.
*
* @param	data	: 	ID of the digital button
*
* @return position where the digital button is found
* 
*/
char AppButtonSearchButton(unsigned char *data);

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static unsigned char statereg = 0x00;
static unsigned char foundButtons[NBROFBUTTONS][12];  
static unsigned char foundTemps[NBROFTEMPS][12];
static unsigned char foundSwitches[NBROFSWITCHES][12];

static unsigned char currentChannel;
static unsigned char LastDiscrepancy;
static unsigned char LastFamilyDiscrepancy;
static unsigned char LastDeviceFlag;

static unsigned char ROM_NO[8];
static unsigned char ROM_Backup[8];

static TASK_HNDL buttontask;
static TASK_HNDL mstask;
static unsigned char mscount = 0;

static APPCAN_MESSAGE msg;
/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
unsigned char AppOWFirst(void)
{
    LastDiscrepancy = 0;
    LastDeviceFlag = 0;
    LastFamilyDiscrepancy = 0;
    return AppSearchRom();
}
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char AppOWNext(void)
{
    return AppSearchRom();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppOWTargetSetup(unsigned char filter)
{
    unsigned char i;
    
    ROM_NO[0] = filter;
    for(i = 1; i < 8; i++)
    {
        ROM_NO[i] = 0;
    }
    LastDiscrepancy = 64;
    LastFamilyDiscrepancy = 0;
    LastDeviceFlag = 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char AppSearchRom(void)
{
    unsigned char id_bit_number;
    unsigned char last_zero, rom_byte_number, search_result;
    unsigned char id_bit = 0;
    unsigned char cmp_id_bit = 0;
    unsigned char rom_byte_mask, search_direction;
    
    //initialise for search
    id_bit_number = 1;
    last_zero = 0;
    rom_byte_number = 0;
    rom_byte_mask = 1;
    search_result = 0;
    search_direction = 0;
    //crc8 = 0;
   
    // If the last call was not the last one
    if (!LastDeviceFlag)
    {        
        StdDS2484OneWireReset(&statereg);
		if(statereg & PRESENCE_DETECT)			// if PPD detected                        
        {			
			StdDS2484OneWireWriteByte(SEARCH_ROM, &statereg);
            
            do
            {
                if(id_bit_number < LastDiscrepancy)
                    search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
                else
                    search_direction = (id_bit_number == LastDiscrepancy);
                StdDS2484OneWireTriplet((search_direction << 7), &statereg);
                id_bit = (statereg >> 5) & 0x01;
                cmp_id_bit = (statereg >> 6) & 0x01;
            
                if(id_bit && cmp_id_bit)  break; 
                if(!id_bit && !cmp_id_bit)                                  //Both bits are 0 ==> Discrepancy
                {
                    if(search_direction == 0)
                    {
                        last_zero = id_bit_number;
                        if(last_zero < 9)
                            LastFamilyDiscrepancy = last_zero;              //last_zero: bit position of the last zero written where there was a discrepancy
                    }
                }
                if(id_bit != cmp_id_bit)    search_direction = id_bit;      //Both bits are different ==> Continue, no discrepancy
            
                if(search_direction == 1)
                    ROM_NO[rom_byte_number] |= rom_byte_mask;
                else
                    ROM_NO[rom_byte_number] &= ~rom_byte_mask;
                id_bit_number++;
                rom_byte_mask <<= 1;
                if(rom_byte_mask == 0)
                {
                    rom_byte_number++;
                    rom_byte_mask = 1;
                }
            }
            while(rom_byte_number < 8);
            if(!(id_bit_number < 65))
            {
                //Search was succesfull
                LastDiscrepancy = last_zero;
                if(LastDiscrepancy == 0)
                    LastDeviceFlag = 1;
                search_result = 1;
            }
        }
        else
        {
            LastDiscrepancy = 0;
            LastDeviceFlag = 0;
            LastFamilyDiscrepancy = 0;
            return 0;
        }
    }
    if(!search_result || !ROM_NO[0])
    {
        LastDiscrepancy = 0;
        LastDeviceFlag = 0;
        LastFamilyDiscrepancy = 0;
        search_result = 0;
    }
    return search_result;
}    
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char AppButtonSearchRomOld(OWFILTER filter, bool usefilter)
{
//    unsigned char i, y, found = 0;
//    unsigned char id_bit;
//    unsigned char cmp_id_bit;
//
//    search_direction = 0;
//    
//    while (!LastDeviceFlag)
//    {
//        for (i=0; i<8; i++)
//        {
//            ROM_NO[i] = 0x00;
//        }
//        StdDS2484OneWireReset(&statereg);
//        if(statereg & PRESENCE_DETECT)			// if PPD detected                        
//        {
//            id_bit_number = 1;
//            last_zero = 0;
//            StdDS2484OneWireWriteByte(SEARCH_ROM, &statereg);
//            
//            for (id_bit_number=1; id_bit_number<65; id_bit_number++)
//            {
//                id_bit = (statereg >> 5) & 0x01;
//                cmp_id_bit = (statereg >> 6) & 0x01;
//                if(id_bit && cmp_id_bit)  break;              //nobody had answered
//                else if(!(id_bit) && !(cmp_id_bit))
//                {
//                    if(id_bit_number == LastDiscrepancy)        search_direction = 0x80;
//                    else if(id_bit_number > LastDiscrepancy)    search_direction = 0x00;
//                    else
//                    {
//                        y = 8 - (id_bit_number - ((id_bit_number / 8) * 8));
//                        search_direction = ROM_Backup[(id_bit_number-1) / 8] << y;
//                    }
//                    if(search_direction == 0x00)
//                    {
//                        last_zero = id_bit_number;
//                        if(last_zero < 9)   LastFamilyDiscrepancy = last_zero;
//                    }
//                }
//                else    search_direction = id_bit;
//
//                id_bit = id_bit << ((id_bit_number - 1) - (((id_bit_number - 1) / 8) * 8));
//                ROM_NO[(id_bit_number-1) / 8] = ROM_NO[(id_bit_number-1) / 8] | id_bit;
//            }
//            LastDiscrepancy = last_zero;
//            for (i=0; i<8; i++)
//            {
//                ROM_Backup[i] = ROM_NO[i];
//            }
//            if(LastDiscrepancy == 0)    LastDeviceFlag = 1;
//            for (i=0; i<8; i++)
//            {
//                foundButtons[found][i] = ROM_NO[i];
//            }
//            found++;
//            // Maximum of [NrButtons] buttons at the same time
//            if (found == NBROFBUTTONS) break;
//        }
//    }
//    LastDiscrepancy = 0;
//    LastFamilyDiscrepancy = 0;
//    LastDeviceFlag = 0;
        return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char checkCRC(unsigned char *id)
{
    unsigned char i;
    unsigned char crc_result;

    crc_result = 0;
    for (i=0; i<8; i++)
    {
        crc_result = CRC[crc_result] ^ id[i];
    }
    if (crc_result == 0)
    {
        // CRC is OK
        return(1);
    }
    else
    {
        // CRC is NOT OK
        return(0);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char findInProcessedButtons(unsigned char *id)
{
    unsigned char i,j;
    for (i = 0; i < NBROFBUTTONS; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (processedButtons[i][j] != id[j])
            {
                // Not this button
                break;
            }
            else if (j == 7)
            {
                // If we get here, it means this button was found
                return i;
            }
        }
    }
   return 0xFF;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppButtonProcessPressed(unsigned char *id)
{
    unsigned int address = 0x2000;
    unsigned char counter=0;
    char data[10];
    unsigned int memLoc;
    do
    {
        Std24LC512TReadPage(address, data, 10);
        if (data[1] == id[1] && data[2] == id[2] && data[3] == id[3] && data[4] == id[4] && data[5] == id[5] && data[6] == id[6])
        {
            // Calculate the memory location of the action for this button
            memLoc = address + 0x10;
            // Store the memory location in RAM
            id[IDMemHigh] = memLoc >> 8;
            id[IDMemLow] = (unsigned char)(memLoc);
            AppButtonCheckPressedAction(id);
            return;
        }
        address += 64;
        counter++;
        
        if((address & 0x00FF) == 0)
        {
            address += 0x0100;
        }
    } while (data[0] != 0xFF && counter < 128);
    // If we get here the button was not found in memory
    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppButtonCheckPressedAction(unsigned char *id)
{
    char data[32];
    
    Std24LC512TReadPage(((unsigned int)(id[IDMemHigh]) << 8) & id[IDMemLow], data, 32);

    if (data[0] == SCENARIOSADDR && AppSfeerCheckSelfLearning(data[1]))
    {
        // First function triggers a SELF LEARNING MOOD
        id[IDTimer] = 100; // 10s
        if (data[17] != 0xFF)
        {
            // Second function is programmed
            id[IDAction] = 22;
        }
        else
        {
            // No second function
            id[IDAction] = 21;
        }
    }
    else if (data[16] == SCENARIOSADDR && AppSfeerCheckSelfLearning(data[17]))
    {
        // Second function triggers a SELF LEARNING MOOD
        id[IDTimer] = 100; // 10s
        id[IDAction] = 23;
    }
    else
    {
        if (data[17] != 0xFF)
        {
            // Start timer for 2nd function
            id[IDTimer] = 5 * module.time2function;
            id[IDAction] = 2;
        }
        else if (data[1] != 0xFF)
        {
            // There is no second function, so we execute the first immediately
            id[IDTimer] = 0;
            id[IDAction] = 1;
            AppButtonExecuteAction(id, 0);
        }
    }
    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppButtonExecuteAction(unsigned char *id, unsigned char release)
{
    unsigned int address = 0x0000;
    char data[32];
    char i, o, function,sfeerToLearn = 0xFF;
    static CMD_INDEX index;

    address = ((unsigned int)(id[IDMemHigh]) << 8) + id[IDMemLow];
    Std24LC512TReadPage(address, data, 32);
    // Function to execute
    switch (id[IDAction])
    {
        case 0:
            return;
        case 1:
        case 2:
            function = id[IDAction] - 1;
            break;
        case 11:
        case 12:
            function = id[IDAction] - 11;
            break;
        case 21:
        case 22:
            sfeerToLearn = 0;
            break;
        case 23:
            sfeerToLearn = 1;
            break;
    }
    // Reset action to 0 to make clear the function was executed
    id[IDAction] = 0;

    // <editor-fold defaultstate="collapsed" desc="Self learning moods">
    if (sfeerToLearn != 0xFF)
    {
        // @TODO: send the self learn mode command to all modules
        // sfeer ID to learn: data[1 + (sfeerToLearn*11)]
    }
    // </editor-fold>

    if (data[1 + (function*16)] == 0xFF) return; // Don't execute empty actions

    if (data[0 + (function*16)] == module.address)
    {
        o = data[1 + (function*16)];                                // te sturen output
    }
    else
    {
        o = 4;
    }
    for (i=0; i<16; i++)
    {
        cmd[o].all[i] = data[i + (function*16)];
    }

    if (cmd[o].Purp.flick != 0)
    {
        cmd[o].Purp.flick = 0;                                                  // make sure flick is always stopped by new action
        rtccount[o].sec = 0;
        rtccount[o].min = 0;
        if (cmd[o].action.actionid != 5 && cmd[o].action.actionid != 6 && cmd[o].action.actionid != 7)
        {
            rtccount[o].flicksec = 0;                                           // set fickersec off IF the new function is not flickering
        }
    }

    // -----------------------------------------------------
    // Check now for special purpose functions
    // -----------------------------------------------------
    switch (cmd[o].action.actionid)
    {
         case 3:                                                                // Dim follower (local or via bus)
            if (release == 0)
            {
                // If the button was pressed, we store if it was 1st or 2nd function so we know what to do when button is released
                id[IDAction] = function + 11;
            }
            else
            {
                // When the button is released, we send STOP DIMMING command
                cmd[o].action.actionid = 4;
            }
            break;
        case 5:                                                                 // Flicker action
        case 6:
        case 7:
            cmd[o].Purp.flick = 1;
            break;
        case 8:                                                                 // Puls follower
            cmd[o].Purp.follow = 1;
            // Reset dim to 0 to make sure command 0 is sent when button is released
            cmd[o].Purp.dim = 0;
            cmd[o].action.actionid = 1;
            cmd[o].action.delayOn.all = 0xFF;
            cmd[o].action.delayOff.all = 0xFF;
            if (release == 0)
            {
                // If the button was pressed, we store if it was 1st or 2nd function so we know what to do when button is released
                id[IDAction] = function + 11;
            }
            else
            {
                // When the button is released, we send the OFF command
                cmd[o].Purp.follow = 0;
                cmd[o].action.actionid = 0;
            }
            break;
    }

    // Check for local or remote action
    for (i=0; i<8; i++)
    {
        data[i] = cmd[o].all[i];
    }
    if (cmd[o].action.cond != 0xFF || cmd[o].action.address > SCENARIOSADDR)
    {
        // Send the conditional or master actions to the master module    
        msg.gpr = 0;
        msg.type = 0x20;
        msg.info = 0;
        msg.address = 0xFF;
        msg.suffix = 0x02;
        msg.data = data;
        msg.length = 8;
        AppCanSend(&msg);
        //DrvCanSend(0, 0x20, 0, 0xFF, 0x02, data, 8);                                   // gpr=0, type=tool, info=2, address=all, suffix=status
    }
    else
    {
        if (cmd[o].action.address == module.address)
        {
            // A local action
            //AppActionMake(o, currentChannel);
            index.o = o;
            index.dsChannel = currentChannel;
            AppActionFillQueue(&index);
            
        }
        else if (cmd[o].action.address == SCENARIOSADDR)
        {
            // Broadcast scenario
            msg.gpr = 0;
            msg.type = 0x7F;
            msg.info = 0;
            msg.address = 0xFF;
            msg.suffix = 0x02;
            msg.data = data;
            msg.length = 8;
            AppCanSend(&msg);
            //DrvCanSend(0, 0x7F, 0, 0xFF, 0x02, data, 8);                            /// gpr, type, info, address, suffix
            // Execute the scenario on this module
            if (cmd[o].action.delayOn.all == 0xFF) 
            {
                //AppScenarioExecute(cmd[o].action.channel);
                AppScenarioFillQueue(&cmd[o].action.channel);
                StdTaskSetPeriod(buttontask, 30000);
                StdTaskStart(buttontask);
                while(StdTaskIsTaskRunning(buttontask)) {}
                //Delay10KTCYx(15);                                                   // 30ms pause om acties wat te vertragen
            }
        }
        else
        {
            // An action on another module
            msg.gpr = 0;
            msg.type = 0x7f;
            msg.info = 0;
            msg.address = cmd[o].action.address;
            msg.suffix = 0x02;
            msg.data = data;
            msg.length = 8;
            AppCanSend(&msg);
            //DrvCanSend(0, 0x7F, 0, cmd[o].action.address, 0x02, data, 8);           // gpr, type, info, address, suffix
            StdTaskSetPeriod(buttontask, 30000);
            StdTaskStart(buttontask);
            while(StdTaskIsTaskRunning(buttontask)) {}                                  // 30ms pause om acties wat te vertragen
            //Delay10KTCYx(15);                                                       
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppButtonTimerTask(void)
{
    StdTaskStop(buttontask);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppButtonMsTask(void)
{
    unsigned char i = 0;
    inter.scan = 1;
    inter.shift >>= 1;
    mscount++;
    if(mscount == 10)
    {
        mscount = 0;
        
        for(i=0; i<NBROFBUTTONS; i++)
        {
            if(processedButtons[i][IDTimer] > 0)
            {
                processedButtons[i][IDTimer]--;
            }
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppButtonToCan(unsigned char *id)
{
    unsigned char dataOut[8], i;

    dataOut[0] = module.address;
    for (i=1; i<8; i++)
    {
        dataOut[i] = id[i];
    }
    msg.gpr = 0;
    msg.type = 0x00;
    msg.info = 0;
    msg.address = 0xFF;
    msg.suffix = 0x20;
    msg.data = (char*)dataOut;
    msg.length = 8;
    AppCanSend(&msg);
    //DrvCanSend(0, 0x00, 0, 0xFF, 0x20, dataOut, 8);		// gpr, type, info, address, suffix
}
/*--------------------------------------------------------------------------------------------------------------------*/
unsigned char AppButtonFindInFound(unsigned char *id)
{
    unsigned char i,j;
    for (i = 0; i < NBROFBUTTONS; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (foundButtons[i][j] != id[j])
            {
                // Not this button
                break;
            }
            else if (j == 7)
            {
                // If we get here, it means this button was found
                return i;
            }
        }
    }
    return 0xFF;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppButtonProcessReleased(unsigned char *id)
{
    unsigned char time;
    if (id[IDAction] == 2)
    {
        if (id[IDTimer] == 0)
        {
            // Timer has ended and 2nd function was not yet executed
            AppButtonExecuteAction(id, 1);
        }
        else
        {
            // Timer has not yet ended, so we execute the first function
            id[IDAction] = 1;
            AppButtonExecuteAction(id, 1);
        }
    }
    else if (id[IDAction] == 11 || id[IDAction] == 12)
    {
        // Stop special commands (dimming - pulse follower)
        AppButtonExecuteAction(id, 1);
    }
    else if (id[IDAction] == 21 || id[IDAction] == 22 || id[IDAction] == 23)
    {
        if (id[IDTimer] == 0)
        {
            // Timer has ended and self learning mood was not reconfigured yet
            AppButtonExecuteAction(id, 1);
        }
        else if (id[IDAction] == 21)
        {
            // There was no second function available so we execute the first
            id[IDAction] = 1;
            AppButtonExecuteAction(id, 1);
        }
        else
        {
            time = 100 - id[IDTimer];
            if (time > 5 * module.time2function)
            {
                // Button was pressed long enough for second function
                id[IDAction] = 2;
                AppButtonExecuteAction(id, 1);
            }
            else
            {
                // Button was not pressed long enough for second function
                id[IDAction] = 1;
                AppButtonExecuteAction(id, 1);
            }
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
//unsigned char testLongPush()
//{
//    unsigned char i;
//
//    for (i=0; i<20; i++)
//    {
//        StdTaskSetPeriod(buttontask, 2000000);
//        StdTaskStart(buttontask);
//        while(StdTaskIsTaskRunning(buttontask)) {};
//        if (MODE == 1)
//        {
//            return 0;
//        }
//    }
//    return 1;
//}
/*--------------------------------------------------------------------------------------------------------------------*/
char AppButtonSearch(unsigned char *data)
{
    unsigned int address = 0x2000;
    unsigned char i,j;
    char IDbutton[8];
    char empty[8];

    for (i=0; i<128; i++)
    {
        Std24LC512TReadPage(address, IDbutton, 8);
        address += 64; 
        if((address & 0x00FF) == 0x0000) address += 0x0100; 
        if (IDbutton[0] == 0xFF || (data[0] == IDbutton[0] && data[1] == IDbutton[1] && data[2] == IDbutton[2] && data[3] == IDbutton[3]) )
        {
            if (IDbutton[0] == 0xFF && i < 127)
            {
                // Write 0xFF to the next line to make sure old buttons are not 'recovered'
                for (j=0; j<8; j++)
                {
                    empty[j] = 0xFF;
                }
                Std24LC512TWritePage(address, empty, 8);
            }
            return i;
        }
    }
    return (char)-1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void progMode(unsigned char output)
{
    char i, position;
    unsigned char IDbutton[8];
    unsigned int address;
    char dataID[8];
    char dataCmd[8];

    AppButtonOneWireReset();
    
    if (processedButtons[0][0] != 0)                   // knop wordt voor het eerst ingelezen
    {
        position = AppButtonSearch(processedButtons[0]);
        if (position != -1)
        {
            address = (0x2000 + ((unsigned int)position * 64));
            for (i=0; i<8; i++)
            {
                dataID[i] = processedButtons[0][i];
            }
            dataCmd[0] = module.address;   // Address
            dataCmd[1] = output;       // Output
            dataCmd[2] = 0x02;          // Action ID
            dataCmd[3] = 0xFF;          // Delay ON
            dataCmd[4] = 0xFF;          // Delay OFF
            dataCmd[5] = 0x64;          // Start value (100%)
            dataCmd[6] = 0xFF;          // Softdim
            dataCmd[7] = 0xFF;          // Condition
                
            Std24LC512TWritePage(address, dataID, 9);           // ID + Regime bewaren
            // Save 1st function
            Std24LC512TWritePage(address + 16, dataCmd, 8);
            // Save 2nd function (manual dimming)
            dataCmd[2] = 0x03;
            Std24LC512TWritePage(address + 32, dataCmd, 8);        
            StdTaskSetPeriod(buttontask, 100000);
            StdTaskStart(buttontask);
            while(StdTaskIsTaskRunning(buttontask)) {};         //100ms pauze

            // Lamp laten flikkeren ter bevestiging
            dim_ch[output].actValue = 0;
            AppDimmerUpdate();
                
            StdTaskSetPeriod(buttontask, 500000);
            StdTaskStart(buttontask);
            while(StdTaskIsTaskRunning(buttontask)) {};         //500ms pauze
                
            dim_ch[output].actValue = 100;
            AppDimmerUpdate();
        }
    }
    
}
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppButtonInit(void)
{
    unsigned char i, result = 0;
    unsigned char found = 0;
    buttontask = StdTaskRegisterTask(30000, AppButtonTimerTask, NULL, 126);
    mstask = StdTaskRegisterTask(10000, AppButtonMsTask, NULL, 121);
    
    StdTaskStart(mstask);
    
    //Execute a searchrom with DS2485 device
    //StdDS2485MasterReset();
    //StdDS2485SearchRom();
    //Looking for all available devices with a fixed family code and store them in a RAM-Array
    //AppOWTargetSetup(0x01);
    //result = AppOWNext();
    //while(result)
//    {
//        for(i=0; i<8; i++)
//        {
//            foundButtons[found][i] = ROM_NO[i];
//        }
//        result = AppOWNext();
//        found++;
//    }
    
    //Looking for all available devices on the bus and store them in a RAM-Array
//    result = AppOWFirst();
//    while(result)
//    {
//        for (i=0; i<8; i++)
//        {
//            foundButtons[found][i] = ROM_NO[i];
//        }
//        result = AppOWNext();
//        found++;
//    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppButtonOneWireReset(void)
{
    unsigned char i, j, pos, found = 0, realChannel;
    unsigned char result = 0;
    
    // <editor-fold defaultstate="collapsed" desc="Clear found buttons">
    for (i=0; i<NBROFBUTTONS; i++)
    {
        for (j=0; j<12; j++)                    //12 Bytes ruimte om een ID op te slaan
        {
            foundButtons[i][j] = 0;
        }
    }
    StdDS2484OneWireReset(&statereg);
    if(statereg & PRESENCE_DETECT)                      // if PPD (presense pulse detect) detected  // see data sheet (p 9)
    {
        AppLEDControl(BUZZY, TRUE, RED);
        AppOWTargetSetup(FILTER_ID_BUTTON);
        result = AppOWNext();
        while(result)
        {
            for(i=0; i<8; i++)
            {
                foundButtons[found][i] = ROM_NO[i];     //Put active button in foundbuttons
            }
            result = AppOWNext();
            found++;
        }
    }
    else if (statereg == 0xFF || (statereg & DEVICE_RESET))
    {
        // Other status was received, which points at an issue with the DS2483
        // 0b00010000 : device was reset (RST)
        // 0xFF : not ack error | write collision | bus collision
        // To resolve this, we write the configuration to the chip again
        StdDS2484WriteConfiguration(0b11100001);                                  // Write Configuration (1WS=0, SPU=0, PDN=0, APU=1
        return;
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Ignore detection of 1 erroneous button">
    if (found == 1 && !checkCRC(foundButtons[0]))
    {
        // If we detect only 1 erroneous button, we ignore the detection
        return;
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Process found buttons">
    if (found > 0)
    {
        for (i=0; i<NBROFBUTTONS; i++)
        {
            // First check CRC of ID and make sure the button ID is not just 0
            if (checkCRC(foundButtons[i]) && foundButtons[i][ID1] != 0)
            {
                pos = findInProcessedButtons(foundButtons[i]);
                if (pos == 0xFF)
                {
                    // First time this button was detected, so we process it
                    if (!module.service)
                    {
                        AppButtonProcessPressed(foundButtons[i]);
                        AppButtonToCan(foundButtons[i]);
                    }
                }
                else
                {
                    // Copy the memory location, timer and action from the processedButtons array to here,
                    // so it doesn't get lost and it remains linked to the same button
                    for (j=8; j<12; j++)
                    {
                        foundButtons[i][j] = processedButtons[pos][j];
                    }
                    // This button was already processed
                    if (foundButtons[i][IDAction] == 2 && foundButtons[i][IDTimer] == 0)
                    {
                        // If the 2nd action was started and the timer has ended, we can execute the 2nd action
                        AppButtonExecuteAction(foundButtons[i], 0);
                    }
                    else if ((foundButtons[i][IDAction] == 21 || foundButtons[i][IDAction] == 22 || foundButtons[i][IDAction] == 23) && foundButtons[i][IDTimer] == 0)
                    {
                        // The timer for a self learning mood has ended, so we execute the self learning action
                        AppButtonExecuteAction(foundButtons[i], 0);
                    }
                }
            }
        }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Process released buttons">
    for (i=0; i<NBROFBUTTONS; i++)
    {
        if (processedButtons[i][0] != 0)
        {
            // Check if this processed button is still pressed
            pos = AppButtonFindInFound(processedButtons[i]);
            if (pos == 0xFF)
            {
                // Button was released
                AppButtonProcessReleased(processedButtons[i]);
            }
        }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Copy found to processed">
    for (i=0; i<NBROFBUTTONS; i++)
    {
        for (j=0; j<12; j++)
        {
            processedButtons[i][j] = foundButtons[i][j];
        }
    }
    // </editor-fold>    
    
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppButtonUpdateButtonsAddress(unsigned char newaddress)
{
    unsigned char High=0x20;
    unsigned char Low=0x00;
    unsigned int address = 0x2000;
    unsigned char counter=0;
    char id[8];
    char data[32];
    unsigned int changed=0;
    do
    {
        //Delay10KTCYx(3);
        StdTaskSetPeriod(buttontask, 6000);
        StdTaskStart(buttontask);
        while(StdTaskIsTaskRunning(buttontask)) {};
        changed = 0;
        Std24LC512TReadPage(address, id, 8);
        if ( id[0] != 0xFF )
        {
            // Als de knop een ID heeft, dan gaan we het adres aanpassen
            Std24LC512TReadPage(address + 0x10, data, 32);          //Lees 22 items
            if (data[0] == module.address)
            {
                changed = 1;
                data[0] = newaddress;
            }
            if (data[16] == module.address)
            {
                changed = 1;
                data[16] = newaddress;
            }
            if (changed == 1)
            {
                // Als de knop geupdatet werd, schrijven we de config opnieuw weg
                Std24LC512TWritePage(address + 0x10, data, 32);
                // Delay tussen schrijven
                //Delay10KTCYx(3);
                StdTaskStart(buttontask);
                while(StdTaskIsTaskRunning(buttontask)) {};
            }
        }
        address += 64;
        counter++;
        if((address & 0x00FF) == 0x00) address += 0x0100;
        
    } while (id[0] != 0xFF && counter < 128);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppButtonclearProcessedButtons(void)
{
    unsigned char i,j;
    // Clear processed buttons
    for (i=0; i<NBROFBUTTONS; i++)
    {
        for (j=0; j<12; j++)
        {
            processedButtons[i][j] = 0;
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool AppButtonProg(void)
{
    unsigned char i, j;
    unsigned char x=0;

    if (module.toolScan)                               // Tool Programmer in use
    {
        module.toolScan = 0;
        AppLEDControl(POWER, false, GREEN);             // Power led UIT
        while (AppServiceGet() == true) {}
        module.toolTimer = 5;    // set timeout to x-sec to receive new Config Data
        module.toolWait = 1;
        AppCaneeReadConfigToCan();
        return true;
    }
    return false;

}
/**********************************************************************************************************************/


