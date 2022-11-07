/*
 * File:   AppScenarios.c
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
#define APP_SCENARIO_QUEUE_LENGTH   10
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
static TASK_HNDL scenariostask;
static unsigned char ar_scenario[APP_SCENARIO_QUEUE_LENGTH];

static unsigned char    appscenariohead = 0;
static unsigned char    appscenariotail = 0;
static bool             appscenariofull = false;
/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppScenariosTimerTask(void)
{
    StdTaskStop(scenariostask);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void AppScenariosInit(void)
{
    scenariostask = StdTaskRegisterTask(10000, AppScenariosTimerTask, NULL, 124);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppScenarioExecute(unsigned char id)
{
    unsigned char i, j, o;
    unsigned int memLoc;
    char data[16];
    
    // Get memory location
    memLoc = 0x8000 + (0x100 * (id - 1));
    
    // Read a maximum of 16 actions from memory
    for (i=0; i<16; i++)
    {
        // Read action from memory
        Std24LC512TReadPage(memLoc, data, 16);
        
        if (data[0] == 0xFF) 
        {
            break;
        }
        else
        {
            // Execute action    
            if (data[Action_Address] == module.address && data[Action_Channel] < NBROFCHANNELS)
            {
                AppActionProcess(data);
                StdTaskSetPeriod(scenariostask, 10000);
                StdTaskStart(scenariostask);
                while(StdTaskIsTaskRunning(scenariostask)) {}
            }
            if (module.master && data[Action_Address] == SCENARIOSADDR) 
            {
                StdTaskSetPeriod(scenariostask, 30000);
                StdTaskStart(scenariostask);
                while(StdTaskIsTaskRunning(scenariostask)) {}
            }
        }
        
        // Skip 16 bytes in memory
        memLoc += 16;
    }
}
  
/*--------------------------------------------------------------------------------------------------------------------*/
bool AppScenarioFillQueue(unsigned char* id)
{
    char diff = 0;
    
    if(appscenariofull == true)
    {
        return false;
    }
    diff = appscenariohead - appscenariotail;
    if(diff < 0)    diff += APP_SCENARIO_QUEUE_LENGTH;
    if(diff >= APP_SCENARIO_QUEUE_LENGTH)   return false;
    ar_scenario[appscenariohead] = *id;
    appscenariohead++;
    if(appscenariohead == APP_SCENARIO_QUEUE_LENGTH)  appscenariohead = 0;
    if(appscenariohead == appscenariotail)    appscenariofull = true;
    return true;
}
/*--------------------------------------------------------------------------------------------------------------------*/
bool AppScenarioReadQueue(unsigned char* id)
{
    if((appscenariohead == appscenariotail)&&(!appscenariofull))         //Receivequeue is empty
    {
        return false;
    }
    *id = ar_scenario[appscenariotail];
    appscenariotail++;
    if(appscenariotail == APP_SCENARIO_QUEUE_LENGTH)  appscenariotail = 0;
    return true;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppScenarioHandler(void)
{
    static unsigned char id = 0;
    
    if(AppScenarioReadQueue(&id))           //Check if there is a scenario available
    {
        AppScenarioExecute(id);
    }
}
/**********************************************************************************************************************/


