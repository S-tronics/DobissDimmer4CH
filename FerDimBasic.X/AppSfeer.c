/*
 * File:   AppSfeer.c
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
//Standard Include Section
#include "..\Source\Standard\I2C\Std24LC512T.h"
#include "..\Source\Standard\Timer\StdTask.h"
//Application Include Section
#include "AppAction.h"
#include "AppDimmer.h"
#include "AppCan.h"
#include "AppSfeer.h"
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
; L O C A L   T Y P E D E F S  &  E N U M S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static TASK_HNDL sfeertask;
static APPCAN_MESSAGE msg;
/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppSfeertTimerTask(void)
{
    StdTaskStop(sfeertask);
}
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppSfeerInit(void)
{
    sfeertask = StdTaskRegisterTask(500000, AppSfeertTimerTask, NULL, 123);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int AppSfeerCheckSelfLearning(unsigned char s)
{
    unsigned int i,j;
    unsigned int address = 0x1220;
    char data[8];
    int selfLearning = 0;

    // Disable selflearning moods
    return 0;

    // Lees bytes met configuratie van de sferen (op zowel master als slave)
    
    Std24LC512TReadPage(address, data, 8);

    // Kijk de correcte bit na om te zien of de sfeer zelflerend is
    i = s / 8;
    j = s % 8;

    selfLearning = (data[i] >> j) & 1;
    return selfLearning;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void sfeerLearn (unsigned char s)
{
    unsigned int i,x;
    unsigned int address = 0x0000;
    static char data[16];
    unsigned char currentState;
    unsigned char status[1];
    unsigned char gpr[1], info[1];
    static CMD_INDEX index;

    enum { Nr, Adr, Out, Act, Don, Doff, Val, Dim, R, G, B, Cond };

    // -----------------------------------------------
    // Bepalen startplaats sfeer
    // -----------------------------------------------
    address = (0x40 + (4 * s)) << 8;    // 0x40 + s*4

    // -----------------------------------------------
    // Controle van sfeerplaatsen
    // -----------------------------------------------
    for (x=0; x<64; x++)
    {
        Std24LC512TReadPage(address, data, 8);

        if (data[Nr] == 0xFF)
            break;
        else
        {
            if (data[Adr] == 'S')
            {
                // Sfeer in sfeer negeren bij zelflerende sfeer
                continue;
            }
            if (data[Adr] == module.address)
            {
                // Status nakijken van eigen uitgang
                currentState = dim_ch[data[Out]].actValue;

                if (currentState < 2)
                {
                    // Relais returns 0 or 1 / Dimmer returns 0 (1% is not taken into account!)
                    if (currentState != data[Act])
                    {
                        // Relais changed state (0 or 1) or Dimmer is now OFF
                        data[Act] = currentState;
                        data[Val] = 0x64;
                        // Nieuwe status van deze uitgang opslaan in de sfeer
                        Std24LC512TWritePage(address, data, 16);
                        // De uitgang tweemaal schakelen om aan te geven dat het aangepast is
                        for (i=0; i<11; i++)
                        {
                            cmd[data[Out]].all[i] = data[i + 1];
                        }
                        cmd[data[Out]].action.actionid = 0x02;           // Action = toggle
                        cmd[data[Out]].action.softDim = 0x00;        // Softspeed = 0 (immediate)
                        index.o = data[Out];
                        index.dsChannel = 2;
                        AppActionFillQueue(&index);
                        //AppActionMake(data[Out], 2);
                        StdTaskSetPeriod(sfeertask, 500000);
                        StdTaskStart(sfeertask);
                        while(StdTaskIsTaskRunning(sfeertask)) {}
                        //Delay10KTCYx(250);			// 500ms pauze
                        cmd[data[Out]].action.actionid = 0x02;           // Action = toggle
                        cmd[data[Out]].action.softDim = 0x00;        // Softspeed = 0 (immediate)
                        index.o = data[Out];
                        index.dsChannel = 2;
                        AppActionFillQueue(&index);
                        //AppActionMake(data[Out], 2);
                    }
                }
                else if (data[Act] != 0x01 || currentState != data[Val])
                {
                    // Het huidige dimniveau is niet hetzelfde als in de sfeer
                    data[Act] = 0x01;
                    data[Val] = currentState;
                    // Nieuwe dimniveau van deze uitgang opslaan in de sfeer
                    Std24LC512TWritePage(address, data, 16);
                    // De uitgang tweemaal schakelen om aan te geven dat het aangepast is
                    for (i=0; i<11; i++)
                    {
                        cmd[data[Out]].all[i] = data[i + 1];
                    }
                    cmd[data[Out]].action.actionid = 0x02;           // Action = toggle
                    cmd[data[Out]].action.softDim = 0x00;        // Softspeed = 0 (immediate)
                    index.o = data[Out];
                    index.dsChannel = 2;
                    AppActionFillQueue(&index);
                    //AppActionMake(data[Out], 2);
                    StdTaskStart(sfeertask);
                    while(StdTaskIsTaskRunning(sfeertask)) {}
                    //Delay10KTCYx(250);			// 500ms pauze
                    cmd[data[Out]].action.actionid = 0x02;           // Action = toggle
                    cmd[data[Out]].action.softDim = 0x00;        // Softspeed = 0 (immediate)
                    index.o = data[Out];
                    index.dsChannel = 2;
                    AppActionFillQueue(&index);
                    //AppActionMake(data[Out], 2);
                }
            }
            else
            {
                // Status opvragen bij module
                msg.gpr = 0;
                msg.type = 0x7F;
                msg.info = 0;
                msg.address = data[Adr];
                msg.suffix = 0x01;
                msg.data = data + 1;
                msg.length = 2;
                AppCanSend(&msg);    
                //DrvCanSend(0, 0x7F, 0, data[Adr], 0x01, data + 1, 2);		// gpr, type, info, address, suffix = 0x01 = status opvragen
                if (AppCanReceiveNext(0x01, &msg))
                {
                    //currentState = status[0];
                    currentState = msg.data[0];

                    if (currentState < 2)
                    {
                        // Relais returns 0 or 1 / Dimmer returns 0 (1% is not taken into account!)
                        if (currentState != data[Act])
                        {
                            // Relais changed state (0 or 1) or Dimmer is now OFF
                            data[Act] = currentState;
                            data[Val] = 0x64;
                            // Nieuwe status van deze uitgang opslaan in de sfeer
                            Std24LC512TWritePage(address, data, 16);
                            // De uitgang tweemaal schakelen om aan te geven dat het aangepast is
                            for (i=0; i<11; i++)
                            {
                                cmd[data[Out]].all[i] = data[i + 1];
                            }
                            // De uitgang tweemaal schakelen om aan te geven dat het aangepast is
                            data[Act] = 0x02;       // Action = toggle
                            data[Dim] = 0x00;       // Speed = 0 (immediate)
                            msg.gpr = 0;
                            msg.type = 0x7F;
                            msg.info = 0;
                            msg.address = data[Adr];
                            msg.suffix = 0x02;
                            msg.data = data + 1;
                            msg.length = 8;
                            AppCanSend(&msg);
                            //DrvCanSend(0, 0x7F, 0, data[Adr], 0x02, data + 1, 8);		// gpr, type, info, address, suffix
                            StdTaskStart(sfeertask);
                            while(StdTaskIsTaskRunning(sfeertask)) {}
                            //Delay10KTCYx(250);			// 500ms pauze
                            msg.gpr = 0;
                            msg.type = 0x7F;
                            msg.info = 0;
                            msg.address = data[Adr];
                            msg.suffix = 0x02;
                            msg.data = data + 1;
                            msg.length = 8;
                            AppCanSend(&msg);
                            //DrvCanSend(0, 0x7F, 0, data[Adr], 0x02, data + 1, 8);		// gpr, type, info, address, suffix
                            StdTaskSetPeriod(sfeertask, 200000);
                            StdTaskStart(sfeertask);
                            while(StdTaskIsTaskRunning(sfeertask)) {}
                            //Delay10KTCYx(100);                  // 200ms pauze (anders loopt volgende status aanvraag fout)
                        }
                    }
                    else if (data[Act] != 0x01 || currentState != data[Val])
                    {
                        // Het huidige dimniveau is niet hetzelfde als in de sfeer
                        data[Act] = 0x01;
                        data[Val] = currentState;
                        // Nieuwe dimniveau van deze uitgang opslaan in de sfeer
                        Std24LC512TWritePage(address, data, 16);
                        // De uitgang tweemaal schakelen om aan te geven dat het aangepast is
                        data[Act] = 0x02;       // Action = toggle
                        data[Dim] = 0x00;       // Speed = 0 (immediate)
                        msg.gpr = 0;
                        msg.type = 0x7F;
                        msg.info = 0;
                        msg.address = data[Adr];
                        msg.suffix = 0x02;
                        msg.data = data + 1;
                        msg.length = 8;
                        AppCanSend(&msg);
                        //DrvCanSend(0, 0x7F, 0, data[Adr], 0x02, data + 1, 8);		// gpr, type, info, address, suffix
                        StdTaskSetPeriod(sfeertask, 250000);
                        StdTaskStart(sfeertask);
                        while(StdTaskIsTaskRunning(sfeertask)) {}
                        //Delay10KTCYx(250);			// 500ms pauze
                        msg.gpr = 0;
                        msg.type = 0x7F;
                        msg.info = 0;
                        msg.address = data[Adr];
                        msg.suffix = 0x02;
                        msg.data = data +1;
                        msg.length = 8;
                        AppCanSend(&msg);
                        //DrvCanSend(0, 0x7F, 0, data[Adr], 0x02, data + 1, 8);		// gpr, type, info, address, suffix
                        StdTaskSetPeriod(sfeertask, 100000);
                        StdTaskStart(sfeertask);
                        while(StdTaskIsTaskRunning(sfeertask)) {}
                        //Delay10KTCYx(100);                  // 200ms pauze (anders loopt volgende status aanvraag fout)
                    }
                }
            }
        }

        // Increase memory counters
        address += 16;
        if((address & 0x00FF) == 0x0000)
        {
            address += 0x0100;
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/


