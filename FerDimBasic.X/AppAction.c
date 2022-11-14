/*
 * File:   AppAction.c
 * Author: stronics
 *
 * Created on 31 July 2021, 13:11
 */

/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "AppConfig.h"
#include "..\Source\System\PIC18F66K80\SysLibAll.h"
//Driver Include Section
#include "..\Source\Driver\PIC18F66K80\DrvCan.h"
//Standard Include Section
//Application Include Section
#include "AppCan.h"
#include "AppDimmer.h"
#include "AppScenarios.h"
#include "AppAction.h"
/**********************************************************************************************************************/

/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define APP_ACTION_QUEUE_LENGTH     10
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   T Y P E D E F S  &  E N U M S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
static void AppActionSetDelayOff (unsigned char o);
static void AppActionSetDelayOn (unsigned char o);
static void AppActionsetRTCTimer(unsigned char channel, unsigned char delay, unsigned char minutes);
static void AppActionCheckActionOn(unsigned char output);
static void AppActionCheckActionOff(unsigned char output);
static void AppActionStopDimming(unsigned char output);
static unsigned char AppActionProcentOfValue(unsigned char channel, unsigned char procent);
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static APPCAN_MESSAGE msg;

static CMD_INDEX        ar_action[APP_ACTION_QUEUE_LENGTH];

static unsigned char    appactionhead = 0;
static unsigned char    appactiontail = 0;
static bool             appactionfull = false;
/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
static void AppActionSetDelayOff(unsigned char o)
{    
    if (dim_ch[o].pulse.all != 0xFF)                
    {
        // Output that are configured for Pulse are priority, also locked outputs use this var
        cmd[o].action.delayOff.delay = dim_ch[o].pulse.delay;
        AppActionsetRTCTimer(o, dim_ch[o].pulse.delay, dim_ch[o].pulse.type);
    }
    else {
        AppActionsetRTCTimer(o, cmd[o].action.delayOff.delay, cmd[o].action.delayOff.type);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void AppActionSetDelayOn(unsigned char o)
{
    AppActionsetRTCTimer(o, cmd[o].action.delayOn.delay, cmd[o].action.delayOn.type);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void AppActionsetRTCTimer(unsigned char channel, unsigned char delay, unsigned char minutes)
{
    if (minutes && delay <= 2)
    {
        // Convert 1 or 2 minutes setting to seconds to achieve higher precision
        minutes = 0;
        delay = delay * 60; // Steps of 1s: 1min = 1s * 60;
    }
    // Set timers in RTCCOUNT registers
    if (!minutes)
    {
        rtccount[channel].sec = delay;
        rtccount[channel].min = 0;
    }
    else if (minutes)
    {
        rtccount[channel].sec = 0;
        // Conversion step for longer timers
        if (delay <= 90) {
            // Delay = 1..90: 1-minute steps
            rtccount[channel].min = delay;
        }
        else if (delay < 118) {
            // Delay = 91..117: 30-minute steps
            rtccount[channel].min = 90 + ((delay-90) * 30);
        }
        else {
            // Delay = 118..126: 60-minute steps
            rtccount[channel].min = 900 + ((delay-117)*60);
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void AppActionCheckActionOff(unsigned char output) 
{   
    unsigned char i, data[16];
    if (dim_ch[output].offAction.address == module.address && dim_ch[output].offAction.channel == output && dim_ch[output].offAction.actionid == 0x01 && dim_ch[output].offAction.delayOn.all == 0xFF) {
        // Don't turn the same output immediately on
        return;
    }
    if (dim_ch[output].offAction.address == module.address && dim_ch[output].offAction.channel == output && dim_ch[output].offAction.actionid == 0x02) {
        // Don't toggle the same output
        return;
    }
    if (dim_ch[output].offAction.address != 0xFF)
    {
        // Copy the action to the local variable (to avoid something could go wrong with VAR in memory)
        for (i=0; i<16; i++) {
            data[i] = dim_ch[output].offAction.all[i];
        }
       AppActionProcess(data);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void AppActionCheckActionOn(unsigned char output) 
{
    unsigned char i, data[16];
    if (cmd[output].action.delayOff.all != 0xFF && dim_ch[output].onAction.address == module.address && dim_ch[output].onAction.channel == output && dim_ch[output].onAction.actionid == 0x00) {
        // Don't override the delayOff from the action by the default one
        return;
    }
    if (dim_ch[output].onAction.address == module.address && dim_ch[output].onAction.channel == output && dim_ch[output].onAction.actionid == 0x00 && dim_ch[output].onAction.delayOff.all == 0xFF) {
        // Don't turn the same output immediately off
        return;
    }
    if (dim_ch[output].onAction.address == module.address && dim_ch[output].onAction.channel == output && dim_ch[output].onAction.actionid == 0x02) {
        // Don't toggle the same output
        return;
    }
    if (dim_ch[output].onAction.address != 0xFF)   
    {
        // Copy the action to the local variable (to avoid something could go wrong with VAR in memory)
        for (i=0; i<16; i++) {
            data[i] = dim_ch[output].onAction.all[i];
        }
        AppActionProcess(data);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void AppActionStopDimming(unsigned char output)
{
    // Make sure manual dimming from either which channel is stopped
    DIMrunning[0][output] = 0;
    DIMrunning[1][output] = 0;
    DIMrunning[2][output] = 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static unsigned char AppActionProcentOfValue(unsigned char channel, unsigned char procent)
{
    unsigned int value;

    value = ((int)dim_ch[channel].actValue * procent) / 100;
    value = dim_ch[channel].actValue - value;

    // if vJumpValue <= endValue we need to set it to the endValue
    if (value <= dim_ch[channel].endValue)
    {
        value = dim_ch[channel].endValue + 1;
    }

    return (unsigned char)value;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/


/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppActionInit(void)
{
    
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppActionProcess(char *action)
{
    unsigned char i, o, data[8];
    static CMD_INDEX index;
    // Set correct channel
    if (action[ACT_ADDRESS] == module.address)
    {
        o = action[ACT_CHANNEL];
    }
    else 
    {
        o = NBROFCHANNELS;
    }
    for (i=0; i<16; i++)
    {
        cmd[o].all[i] = action[i];
    }
    
    // Reset timers to stop the ongoing blinking functions
    if (cmd[o].Purp.flick != 0)
    {
        cmd[o].Purp.flick = 0;
        rtccount[o].sec = 0;
        rtccount[o].min = 0;
        if (cmd[o].action.actionid != 5 && cmd[o].action.actionid != 6 && cmd[o].action.actionid != 7)
        {
            rtccount[o].flicksec = 0;                                           // set fickersec off IF the new function is not flickering
        }
    }
    
    // Check for special actions
    switch (cmd[o].action.actionid)
    {
        case 5:                                                                 // Flicker action
        case 6:
        case 7:
            cmd[o].Purp.flick = 1;
            break;
    }
    
    // Correct the value
    if (cmd[o].action.value > 100) cmd[o].action.value = 100;

    // Check for local or remote action
    if ((cmd[o].action.cond > 0 && cmd[o].action.cond < 0xFF) || cmd[o].action.address > SCENARIOSADDR)
    {
        // Send the conditional or master actions to the master module    
        msg.gpr = 0;
        msg.type = 0x20;
        msg.info = 0;
        msg.address = 0xFF;
        msg.suffix = 0x02;
        msg.data = action;
        msg.length = 8;
        AppCanSend(&msg);
    }
    else
    {
        if (cmd[o].action.address == module.address)
        {
            // A local action
            //AppActionMake(o, 2);
            index.o = o;
            index.dsChannel = 2;
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
            msg.data = action;
            msg.length = 8;
            AppCanSend(&msg);                  
            // Execute the scenario on this module
            if (cmd[o].action.delayOn.all == 0xFF) 
            {
                //AppScenarioExecute(cmd[o].action.channel); 
                AppScenarioFillQueue(&cmd[o].action.channel);
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
            msg.data = action;
            msg.length = 8;
            AppCanSend(&msg);
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool AppActionFillQueue(CMD_INDEX* index)
{
    signed char diff = 0;
    
    if(appactionfull == true)
    {
        return false;
    }
    diff = appactionhead - appactiontail;
    if(diff < 0)    diff += APP_ACTION_QUEUE_LENGTH;
    if(diff >= APP_ACTION_QUEUE_LENGTH)   return false;
    ar_action[appactionhead] = *index;
    appactionhead++;
    if(appactionhead == APP_ACTION_QUEUE_LENGTH)  appactionhead = 0;
    if(appactionhead == appactiontail)    appactionfull = true;
    return true;
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool AppActionReadQueue(CMD_INDEX* index)
{
    if((appactionhead == appactiontail)&&(!appactionfull))         //Receivequeue is empty
    {
        return false;
    }
    *index = ar_action[appactiontail];
    appactiontail++;
    if(appactiontail == APP_ACTION_QUEUE_LENGTH)  appactiontail = 0;
    return true;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppActionHandler(void)
{
    static CMD_INDEX index;
    
    index.dsChannel = 0;
    index.o = 0;
    
    if(AppActionReadQueue(&index))
    {
        AppActionMake(index.o, index.dsChannel);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppActionMake(unsigned char o, unsigned char dsChannel)
{
    unsigned char i;
    static CMD_INDEX index;
    // Correct the value
    if (cmd[o].action.value > 100) cmd[o].action.value = 100;
    
    // <editor-fold defaultstate="collapsed" desc="Flicker action">
    if (cmd[o].Purp.flick && dim_ch[o].pulse.all == 0xFF)
    {
        switch (cmd[o].action.actionid)
        {
            case 5:
                rtccount[o].oldstatus = 100;                                    // Blinking ends in ON
                break;
            case 6:
                rtccount[o].oldstatus = 0;                                      // Blinking ends in OFF
                break;
            case 7:
                if (!rtccount[o].flicksec)                                      // Only set START STATE when blinking has not started yet!
                {
                    rtccount[o].oldstatus = dim_ch[o].actValue;                    // Blinking ends in START STATE
                }
                break;
        }
        rtccount[o].flicksec = 1;                                               // start blinking within 1 second.
        if (!cmd[o].action.delayOff.type)
        {
            rtccount[o].sec = cmd[o].action.delayOff.delay;
            rtccount[o].min = 0;
        }
        else if (cmd[o].action.delayOff.type)
        {
            rtccount[o].sec = 0;
            rtccount[o].min = cmd[o].action.delayOff.delay;
        }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="0 = OFF">
    else if (cmd[o].action.actionid == 0)						// OFF
    {
        if (dim_ch[o].actValue > 0 && dim_ch[o].pulse.all != 0xFF)                    
        {
            cmd[o].action.delayOff.delay = dim_ch[o].pulse.delay;                  // ... then do not refresh puls, but set pulse delay in delayed off value (otherwise it will be overwritten with value now in CMD)
        }
        else if (cmd[o].action.delayOff.all == 0xFF) AppActionSetOff(o);
        else AppActionSetDelayOff(o);
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="1 = ON">
    else if (cmd[o].action.actionid == 1)                                                // ON
    {
        if (dim_ch[o].actValue > 0 && dim_ch[o].pulse.all != 0xFF)                   // if output is configured as puls ...
        {
            cmd[o].action.delayOff.delay = dim_ch[o].pulse.delay;                  // ... then do not refresh puls, but do this
        }
        else if (dim_ch[o].actValue != cmd[o].action.value)
        {
            if (cmd[o].action.delayOn.all == 0xFF)
            {
                AppActionSetOn(o);
            }
            else 
            {
                AppActionSetDelayOn(o);
            }
        }
        else
        {
            if (cmd[o].action.delayOff.all != 0xFF)
            {
                AppActionSetDelayOff(o);
            }
        }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="2 = TOGGLE">
    else if (cmd[o].action.actionid == 2)						// Toggle
    {
        if (dim_ch[o].actValue > 0 && dim_ch[o].pulse.all == 0xFF && (dim_ch[o].moving == NO || dim_ch[o].moving == UP))
        {
            cmd[o].action.actionid = 0;
            AppActionSetOff(o);                                                    // toggle from ON to OFF should always be executed immediately, no matter what delay is configured
        }
        else if (dim_ch[o].actValue > 0 && dim_ch[o].pulse.all != 0xFF)               // if output is configured as puls ...
        {
            cmd[o].action.delayOff.delay = dim_ch[o].pulse.delay;                  // ... then do not refresh puls, but do this
        }
        else if ((dim_ch[o].actValue > 0 && dim_ch[o].moving == DOWN) || dim_ch[o].actValue == 0)
        {
            if (cmd[o].action.delayOn.all == 0xFF)
            {
                AppActionSetOn(o);
            }
            else 
            {
                AppActionSetDelayOn(o);
            }
        }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="3 = START DIMMING">
    else if (cmd[o].action.actionid == 3)                                                // DIM running
    {
        dim_ch[o].moving = NO;                                                     // stop moving DIM running for this channel
        DIMrunning[dsChannel][o] = dim_ch[o].direction;                            // start DIMrunning in last direction for this channel of this dsChannel (dsChannel = 4 is BUS commando or EVOLUTION mode)
        dim_ch[o].speed = dim_ch[o].runSpeed;                                         // set DIM running speed
        if (dim_ch[o].actValue == 0)
        {
            msg.gpr = 0;
            msg.type = 0x00;
            msg.info = 2;
            msg.address = 0xFF;
            msg.suffix = 0x01;
            msg.data[0] = module.address;
            msg.data[1] = o;
            msg.data[2] = 1;
            AppCanSend(&msg);                        //Send status update via CAN-bus
        }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="4 = STOP DIMMING">
    else if (cmd[o].action.actionid == 4)                                       // DIM running STOP
    {
        dim_ch[o].moving = NO;                                                  // stop moving DIM running for this channel
        DIMrunning[dsChannel][o] = 0;   
        // stop DIMrunning UP for this channel of this dsChannel (dsChannel = 4 is BUS commando or EVOLUTION mode)
        
        //Send Status Update
        msg.gpr = 0;
        msg.type = 0x00;
        msg.info = 2;
        msg.address = 0xFF;
        msg.suffix = 0x01;
        msg.data[0] = module.address;
        msg.data[1] = o;
        msg.data[2] = dim_ch[0].actValue;
        msg.length = 3;
        AppCanSend(&msg);                          
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="9 = PIR ON">
    else if (cmd[o].action.actionid == 9)                                                // PIR On
    {
        if (dim_ch[o].actValue == 0                                                // Turn output ON if the output is OFF
            || (dim_ch[o].actValue > 0 && (rtccount[o].min != 0 || rtccount[o].sec != 0))) // ... or refresh the timer if it has a delayed OFF
        {
            cmd[o].action.actionid = 1;
            //(o, dsChannel);
        }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Synchronized output">
    if (dim_ch[o].syncOutput != 0xFF)
    {
        for (i = 0; i < 17; i++)
        {
            cmd[dim_ch[o].syncOutput].all[i] = cmd[o].all[i];
        }
        if (dim_ch[dim_ch[o].syncOutput].syncOutput == o)
        {
            // Avoid looping when sync is set vice versa
            dim_ch[dim_ch[o].syncOutput].syncOutput = 0xFF; // temporarily disable sync output to avoid looping
            index.o = dim_ch[o].syncOutput;
            index.dsChannel = dsChannel;
            AppActionFillQueue(&index);
            //AppActionMake(dim_ch[o].syncOutput, dsChannel);
            dim_ch[dim_ch[o].syncOutput].syncOutput = o; // restore sync output
        }
        else
        {
            index.o = dim_ch[o].syncOutput;
            index.dsChannel = dsChannel;
            AppActionFillQueue(&index);
            //AppActionMake(dim_ch[o].syncOutput, dsChannel);
        }
        
    }
    // </editor-fold>    
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppActionSetOn(unsigned char o)
{
    unsigned char changed = 0;
    static CMD_INDEX index;
    
    AppActionStopDimming(o);
    if (dim_ch[o].actValue == 0) 
    {
        changed = 1;
    }
    
    // <editor-fold defaultstate="collapsed" desc="LOCKS">
    // If the action is ON and the output is locked with other outputs
    if (dim_ch[o].lock1 < 4 || dim_ch[o].lock2 < 4)
    {
        // Turn off the locked outputs before continuing
        if (dim_ch[o].lock1 < 4 && dim_ch[dim_ch[o].lock1].actValue != 0)
        {
            AppActionSetOff(dim_ch[o].lock1);                                         // Only turn the exit OFF when it is ON
            cmd[o].action.delayOn.delay = 2;                                    // Delay the ON command between 1 - 2 seconds to save the engine
            cmd[o].action.delayOn.type = 0;
            index.o = o;
            index.dsChannel = 2;
            AppActionFillQueue(&index);
            //AppActionMake(o, 2);
            return;
        }
        if (dim_ch[o].lock2 < 4 && dim_ch[dim_ch[o].lock2].actValue != 0)
        {
            AppActionSetOff(dim_ch[o].lock2);                                         // Only turn the exit OFF when it is ON
            cmd[o].action.delayOn.delay = 2;                                    // Delay the ON command between 1 - 2 seconds to save the engine
            cmd[o].action.delayOn.type = 0;
            index.o = o;
            index.dsChannel = 2;
            AppActionFillQueue(&index);
            //AppActionMake(o, 2);
            return;
        }
    }
    // </editor-fold>
        
    if (dim_ch[o].actValue == 0)                                                   // SET START LEVEL
    {
        dim_ch[o].actValue = dim_ch[o].vStart;                                        // Always start at vStart
        AppDimmerUpdate();                                                        // Send update now

        if (cmd[o].action.value < dim_ch[o].vStart)                                // If we need to go to a lower % we will add a delayed ON action to go further down
        {
            cmd[o].action.delayOn.type = 0;
            cmd[o].action.delayOn.delay = 2;
            AppActionSetDelayOn(o);
            return;
        }
    }

    dim_ch[o].endValue = cmd[o].action.value;
    if (dim_ch[o].actValue > dim_ch[o].endValue)                                      // MOVE DOWN
    {
        dim_ch[o].moving = DOWN;
        dim_ch[o].actValue = AppActionProcentOfValue(o, dim_ch[o].vJump);                      // Jump down immediately with vJump
        AppDimmerUpdate();
        if (cmd[o].action.softDim != 255) dim_ch[o].tDown = cmd[o].action.softDim; // Soft stop from action...
        else dim_ch[o].tDown = dim_ch[o].tDowndef;                                    // ... or from default value
        dim_ch[o].speed = dim_ch[o].tDown;
    }
    else if (dim_ch[o].actValue < dim_ch[o].endValue)                                 // MOVE UP
    {
        dim_ch[o].moving = UP;
        if (cmd[o].action.softDim != 255) dim_ch[o].tUp = cmd[o].action.softDim;   // Soft start from action...
        else dim_ch[o].tUp = dim_ch[o].tUpdef;                                        // ... or from default value
        dim_ch[o].speed = dim_ch[o].tUp;
    }

    if (dim_ch[o].speed == 0)                                                      // No soft start/stop
    {
        dim_ch[o].moving = NO;
        dim_ch[o].actValue = dim_ch[o].endValue;                                      // Turn ON immediately        
        AppDimmerUpdate();
    }

    // <editor-fold defaultstate="collapsed" desc="RESET RTC COUNTERS">
    // Reset the RTC counters, because otherwise the check for PIR ON looks at old information and the PIR action will take over
    if (cmd[o].Purp.flick == 0)
    {
        rtccount[o].sec = 0;
        rtccount[o].min = 0;
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="DELAYED OFF">
    // Action ON and NOT flickering AND there is a delayed OFF defined
    if (cmd[o].Purp.flick == 0 && (cmd[o].action.delayOff.all != 0xFF || dim_ch[o].pulse.all != 0xFF))
    {
        AppActionSetDelayOff(o);
    }
    // </editor-fold>
    
    if (changed == 1 || (dim_ch[o].onAction.address == module.address && dim_ch[o].onAction.channel == o)) {
        // Check the Action ON ENABLE only when status has changed OR if the output should turn off itself (otherwise it will never turn off because timers were reset)
        AppActionCheckActionOn(o);
    }
    msg.gpr = 0;
    msg.type = 0x00;
    msg.info = 2;
    msg.address = 0xFF;
    msg.suffix = 0x01;
    msg.data[0] = module.address;
    msg.data[1] = o;
    msg.data[2] = dim_ch[0].endValue;
    AppCanSend(&msg);                            //Send status update via CAN-bus                         
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppActionSetOff(unsigned char o)
{
    unsigned char changed = 0;
    
    AppActionStopDimming(o);
    
    if (dim_ch[o].actValue > 0) {
        changed = 1;
    }
    
    dim_ch[o].endValue = 0;                                                        // OFF = 0

    if (dim_ch[o].actValue > dim_ch[o].endValue)                                      // MOVE DOWN
    {
        dim_ch[o].moving = DOWN;
        dim_ch[o].actValue = AppActionProcentOfValue(o, dim_ch[o].vJump);                      // Jump down immediately with vJump%
        AppDimmerUpdate();                                                        
        if (cmd[o].action.softDim != 255) dim_ch[o].tDown = cmd[o].action.softDim; // Soft stop from action...
        else dim_ch[o].tDown = dim_ch[o].tDowndef;                                    // ... or from default value
        dim_ch[o].speed = dim_ch[o].tDown;
    }

    if (dim_ch[o].speed == 0)                                                      // No soft stop
    {
        dim_ch[o].actValue = 0;                                                    // Turn OFF immediately
        dim_ch[o].moving = NO;
        AppDimmerUpdate();
    }
    
    if (changed == 1) {
        AppActionCheckActionOff(o);                                                      // Execute the action on enable
    }
    // Send status update via CAN
    msg.gpr = 0;
    msg.type = 0x00;
    msg.info = 2;
    msg.address = 0xFF;
    msg.suffix = 0x01;
    msg.data[0] = module.address;
    msg.data[1] = o;
    msg.data[2] = dim_ch[0].endValue;
    AppCanSend(&msg);                                    //Send status update via CAN-bus                        
}
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/
