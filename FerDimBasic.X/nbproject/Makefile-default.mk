#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/FerDimBasic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/FerDimBasic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../Source/Driver/PIC18F66K80/DrvCCP.c ../Source/Driver/PIC18F66K80/DrvEeprom.c ../Source/Driver/PIC18F66K80/DrvGpio.c ../Source/Driver/PIC18F66K80/DrvI2C.c ../Source/Driver/PIC18F66K80/DrvString.c ../Source/Driver/PIC18F66K80/DrvTimer.c ../Source/Driver/PIC18F66K80/DrvTimerTick.c ../Source/Driver/PIC18F66K80/DrvCan.c ../Source/Standard/Eeprom/StdEeprom.c ../Source/Standard/I2C/Std24LC512T.c ../Source/Standard/I2C/StdDS2484.c ../Source/Standard/Timer/StdTask.c ../Source/Standard/I2C/StdDS2485.c ../Source/System/PIC18F66K80/System.c AppIrq.c AppZCD.c main.c AppButton.c AppCan.c AppDimmer.c AppEeprom.c AppLed.c AppAction.c AppScenarios.c AppSfeer.c AppService.c AppRTC.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1711380212/DrvCCP.p1 ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1 ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1 ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1 ${OBJECTDIR}/_ext/1711380212/DrvString.p1 ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1 ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1 ${OBJECTDIR}/_ext/1711380212/DrvCan.p1 ${OBJECTDIR}/_ext/499713773/StdEeprom.p1 ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1 ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1 ${OBJECTDIR}/_ext/445730488/StdTask.p1 ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1 ${OBJECTDIR}/_ext/962826093/System.p1 ${OBJECTDIR}/AppIrq.p1 ${OBJECTDIR}/AppZCD.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/AppButton.p1 ${OBJECTDIR}/AppCan.p1 ${OBJECTDIR}/AppDimmer.p1 ${OBJECTDIR}/AppEeprom.p1 ${OBJECTDIR}/AppLed.p1 ${OBJECTDIR}/AppAction.p1 ${OBJECTDIR}/AppScenarios.p1 ${OBJECTDIR}/AppSfeer.p1 ${OBJECTDIR}/AppService.p1 ${OBJECTDIR}/AppRTC.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1711380212/DrvCCP.p1.d ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1.d ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1.d ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1.d ${OBJECTDIR}/_ext/1711380212/DrvString.p1.d ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1.d ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1.d ${OBJECTDIR}/_ext/1711380212/DrvCan.p1.d ${OBJECTDIR}/_ext/499713773/StdEeprom.p1.d ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1.d ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1.d ${OBJECTDIR}/_ext/445730488/StdTask.p1.d ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1.d ${OBJECTDIR}/_ext/962826093/System.p1.d ${OBJECTDIR}/AppIrq.p1.d ${OBJECTDIR}/AppZCD.p1.d ${OBJECTDIR}/main.p1.d ${OBJECTDIR}/AppButton.p1.d ${OBJECTDIR}/AppCan.p1.d ${OBJECTDIR}/AppDimmer.p1.d ${OBJECTDIR}/AppEeprom.p1.d ${OBJECTDIR}/AppLed.p1.d ${OBJECTDIR}/AppAction.p1.d ${OBJECTDIR}/AppScenarios.p1.d ${OBJECTDIR}/AppSfeer.p1.d ${OBJECTDIR}/AppService.p1.d ${OBJECTDIR}/AppRTC.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1711380212/DrvCCP.p1 ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1 ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1 ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1 ${OBJECTDIR}/_ext/1711380212/DrvString.p1 ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1 ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1 ${OBJECTDIR}/_ext/1711380212/DrvCan.p1 ${OBJECTDIR}/_ext/499713773/StdEeprom.p1 ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1 ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1 ${OBJECTDIR}/_ext/445730488/StdTask.p1 ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1 ${OBJECTDIR}/_ext/962826093/System.p1 ${OBJECTDIR}/AppIrq.p1 ${OBJECTDIR}/AppZCD.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/AppButton.p1 ${OBJECTDIR}/AppCan.p1 ${OBJECTDIR}/AppDimmer.p1 ${OBJECTDIR}/AppEeprom.p1 ${OBJECTDIR}/AppLed.p1 ${OBJECTDIR}/AppAction.p1 ${OBJECTDIR}/AppScenarios.p1 ${OBJECTDIR}/AppSfeer.p1 ${OBJECTDIR}/AppService.p1 ${OBJECTDIR}/AppRTC.p1

# Source Files
SOURCEFILES=../Source/Driver/PIC18F66K80/DrvCCP.c ../Source/Driver/PIC18F66K80/DrvEeprom.c ../Source/Driver/PIC18F66K80/DrvGpio.c ../Source/Driver/PIC18F66K80/DrvI2C.c ../Source/Driver/PIC18F66K80/DrvString.c ../Source/Driver/PIC18F66K80/DrvTimer.c ../Source/Driver/PIC18F66K80/DrvTimerTick.c ../Source/Driver/PIC18F66K80/DrvCan.c ../Source/Standard/Eeprom/StdEeprom.c ../Source/Standard/I2C/Std24LC512T.c ../Source/Standard/I2C/StdDS2484.c ../Source/Standard/Timer/StdTask.c ../Source/Standard/I2C/StdDS2485.c ../Source/System/PIC18F66K80/System.c AppIrq.c AppZCD.c main.c AppButton.c AppCan.c AppDimmer.c AppEeprom.c AppLed.c AppAction.c AppScenarios.c AppSfeer.c AppService.c AppRTC.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/FerDimBasic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F46K80
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1711380212/DrvCCP.p1: ../Source/Driver/PIC18F66K80/DrvCCP.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvCCP.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvCCP.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvCCP.p1 ../Source/Driver/PIC18F66K80/DrvCCP.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvCCP.d ${OBJECTDIR}/_ext/1711380212/DrvCCP.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvCCP.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1: ../Source/Driver/PIC18F66K80/DrvEeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1 ../Source/Driver/PIC18F66K80/DrvEeprom.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvEeprom.d ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvGpio.p1: ../Source/Driver/PIC18F66K80/DrvGpio.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1 ../Source/Driver/PIC18F66K80/DrvGpio.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvGpio.d ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvI2C.p1: ../Source/Driver/PIC18F66K80/DrvI2C.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1 ../Source/Driver/PIC18F66K80/DrvI2C.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvI2C.d ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvString.p1: ../Source/Driver/PIC18F66K80/DrvString.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvString.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvString.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvString.p1 ../Source/Driver/PIC18F66K80/DrvString.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvString.d ${OBJECTDIR}/_ext/1711380212/DrvString.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvString.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvTimer.p1: ../Source/Driver/PIC18F66K80/DrvTimer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1 ../Source/Driver/PIC18F66K80/DrvTimer.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvTimer.d ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1: ../Source/Driver/PIC18F66K80/DrvTimerTick.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1 ../Source/Driver/PIC18F66K80/DrvTimerTick.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.d ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvCan.p1: ../Source/Driver/PIC18F66K80/DrvCan.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvCan.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvCan.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvCan.p1 ../Source/Driver/PIC18F66K80/DrvCan.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvCan.d ${OBJECTDIR}/_ext/1711380212/DrvCan.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvCan.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/499713773/StdEeprom.p1: ../Source/Standard/Eeprom/StdEeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/499713773" 
	@${RM} ${OBJECTDIR}/_ext/499713773/StdEeprom.p1.d 
	@${RM} ${OBJECTDIR}/_ext/499713773/StdEeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/499713773/StdEeprom.p1 ../Source/Standard/Eeprom/StdEeprom.c 
	@-${MV} ${OBJECTDIR}/_ext/499713773/StdEeprom.d ${OBJECTDIR}/_ext/499713773/StdEeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/499713773/StdEeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1: ../Source/Standard/I2C/Std24LC512T.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2038438061" 
	@${RM} ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1 ../Source/Standard/I2C/Std24LC512T.c 
	@-${MV} ${OBJECTDIR}/_ext/2038438061/Std24LC512T.d ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2038438061/StdDS2484.p1: ../Source/Standard/I2C/StdDS2484.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2038438061" 
	@${RM} ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1 ../Source/Standard/I2C/StdDS2484.c 
	@-${MV} ${OBJECTDIR}/_ext/2038438061/StdDS2484.d ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/445730488/StdTask.p1: ../Source/Standard/Timer/StdTask.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/445730488" 
	@${RM} ${OBJECTDIR}/_ext/445730488/StdTask.p1.d 
	@${RM} ${OBJECTDIR}/_ext/445730488/StdTask.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/445730488/StdTask.p1 ../Source/Standard/Timer/StdTask.c 
	@-${MV} ${OBJECTDIR}/_ext/445730488/StdTask.d ${OBJECTDIR}/_ext/445730488/StdTask.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/445730488/StdTask.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2038438061/StdDS2485.p1: ../Source/Standard/I2C/StdDS2485.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2038438061" 
	@${RM} ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1 ../Source/Standard/I2C/StdDS2485.c 
	@-${MV} ${OBJECTDIR}/_ext/2038438061/StdDS2485.d ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/962826093/System.p1: ../Source/System/PIC18F66K80/System.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/962826093" 
	@${RM} ${OBJECTDIR}/_ext/962826093/System.p1.d 
	@${RM} ${OBJECTDIR}/_ext/962826093/System.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/962826093/System.p1 ../Source/System/PIC18F66K80/System.c 
	@-${MV} ${OBJECTDIR}/_ext/962826093/System.d ${OBJECTDIR}/_ext/962826093/System.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/962826093/System.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppIrq.p1: AppIrq.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppIrq.p1.d 
	@${RM} ${OBJECTDIR}/AppIrq.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppIrq.p1 AppIrq.c 
	@-${MV} ${OBJECTDIR}/AppIrq.d ${OBJECTDIR}/AppIrq.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppIrq.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppZCD.p1: AppZCD.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppZCD.p1.d 
	@${RM} ${OBJECTDIR}/AppZCD.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppZCD.p1 AppZCD.c 
	@-${MV} ${OBJECTDIR}/AppZCD.d ${OBJECTDIR}/AppZCD.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppZCD.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppButton.p1: AppButton.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppButton.p1.d 
	@${RM} ${OBJECTDIR}/AppButton.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppButton.p1 AppButton.c 
	@-${MV} ${OBJECTDIR}/AppButton.d ${OBJECTDIR}/AppButton.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppButton.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppCan.p1: AppCan.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppCan.p1.d 
	@${RM} ${OBJECTDIR}/AppCan.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppCan.p1 AppCan.c 
	@-${MV} ${OBJECTDIR}/AppCan.d ${OBJECTDIR}/AppCan.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppCan.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppDimmer.p1: AppDimmer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppDimmer.p1.d 
	@${RM} ${OBJECTDIR}/AppDimmer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppDimmer.p1 AppDimmer.c 
	@-${MV} ${OBJECTDIR}/AppDimmer.d ${OBJECTDIR}/AppDimmer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppDimmer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppEeprom.p1: AppEeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppEeprom.p1.d 
	@${RM} ${OBJECTDIR}/AppEeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppEeprom.p1 AppEeprom.c 
	@-${MV} ${OBJECTDIR}/AppEeprom.d ${OBJECTDIR}/AppEeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppEeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppLed.p1: AppLed.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppLed.p1.d 
	@${RM} ${OBJECTDIR}/AppLed.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppLed.p1 AppLed.c 
	@-${MV} ${OBJECTDIR}/AppLed.d ${OBJECTDIR}/AppLed.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppLed.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppAction.p1: AppAction.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppAction.p1.d 
	@${RM} ${OBJECTDIR}/AppAction.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppAction.p1 AppAction.c 
	@-${MV} ${OBJECTDIR}/AppAction.d ${OBJECTDIR}/AppAction.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppAction.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppScenarios.p1: AppScenarios.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppScenarios.p1.d 
	@${RM} ${OBJECTDIR}/AppScenarios.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppScenarios.p1 AppScenarios.c 
	@-${MV} ${OBJECTDIR}/AppScenarios.d ${OBJECTDIR}/AppScenarios.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppScenarios.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppSfeer.p1: AppSfeer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppSfeer.p1.d 
	@${RM} ${OBJECTDIR}/AppSfeer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppSfeer.p1 AppSfeer.c 
	@-${MV} ${OBJECTDIR}/AppSfeer.d ${OBJECTDIR}/AppSfeer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppSfeer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppService.p1: AppService.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppService.p1.d 
	@${RM} ${OBJECTDIR}/AppService.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppService.p1 AppService.c 
	@-${MV} ${OBJECTDIR}/AppService.d ${OBJECTDIR}/AppService.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppService.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppRTC.p1: AppRTC.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppRTC.p1.d 
	@${RM} ${OBJECTDIR}/AppRTC.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=pickit3   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppRTC.p1 AppRTC.c 
	@-${MV} ${OBJECTDIR}/AppRTC.d ${OBJECTDIR}/AppRTC.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppRTC.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1711380212/DrvCCP.p1: ../Source/Driver/PIC18F66K80/DrvCCP.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvCCP.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvCCP.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvCCP.p1 ../Source/Driver/PIC18F66K80/DrvCCP.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvCCP.d ${OBJECTDIR}/_ext/1711380212/DrvCCP.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvCCP.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1: ../Source/Driver/PIC18F66K80/DrvEeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1 ../Source/Driver/PIC18F66K80/DrvEeprom.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvEeprom.d ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvEeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvGpio.p1: ../Source/Driver/PIC18F66K80/DrvGpio.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1 ../Source/Driver/PIC18F66K80/DrvGpio.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvGpio.d ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvGpio.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvI2C.p1: ../Source/Driver/PIC18F66K80/DrvI2C.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1 ../Source/Driver/PIC18F66K80/DrvI2C.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvI2C.d ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvI2C.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvString.p1: ../Source/Driver/PIC18F66K80/DrvString.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvString.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvString.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvString.p1 ../Source/Driver/PIC18F66K80/DrvString.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvString.d ${OBJECTDIR}/_ext/1711380212/DrvString.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvString.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvTimer.p1: ../Source/Driver/PIC18F66K80/DrvTimer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1 ../Source/Driver/PIC18F66K80/DrvTimer.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvTimer.d ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvTimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1: ../Source/Driver/PIC18F66K80/DrvTimerTick.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1 ../Source/Driver/PIC18F66K80/DrvTimerTick.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.d ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvTimerTick.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1711380212/DrvCan.p1: ../Source/Driver/PIC18F66K80/DrvCan.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1711380212" 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvCan.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1711380212/DrvCan.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1711380212/DrvCan.p1 ../Source/Driver/PIC18F66K80/DrvCan.c 
	@-${MV} ${OBJECTDIR}/_ext/1711380212/DrvCan.d ${OBJECTDIR}/_ext/1711380212/DrvCan.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1711380212/DrvCan.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/499713773/StdEeprom.p1: ../Source/Standard/Eeprom/StdEeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/499713773" 
	@${RM} ${OBJECTDIR}/_ext/499713773/StdEeprom.p1.d 
	@${RM} ${OBJECTDIR}/_ext/499713773/StdEeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/499713773/StdEeprom.p1 ../Source/Standard/Eeprom/StdEeprom.c 
	@-${MV} ${OBJECTDIR}/_ext/499713773/StdEeprom.d ${OBJECTDIR}/_ext/499713773/StdEeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/499713773/StdEeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1: ../Source/Standard/I2C/Std24LC512T.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2038438061" 
	@${RM} ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1 ../Source/Standard/I2C/Std24LC512T.c 
	@-${MV} ${OBJECTDIR}/_ext/2038438061/Std24LC512T.d ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2038438061/Std24LC512T.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2038438061/StdDS2484.p1: ../Source/Standard/I2C/StdDS2484.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2038438061" 
	@${RM} ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1 ../Source/Standard/I2C/StdDS2484.c 
	@-${MV} ${OBJECTDIR}/_ext/2038438061/StdDS2484.d ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2038438061/StdDS2484.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/445730488/StdTask.p1: ../Source/Standard/Timer/StdTask.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/445730488" 
	@${RM} ${OBJECTDIR}/_ext/445730488/StdTask.p1.d 
	@${RM} ${OBJECTDIR}/_ext/445730488/StdTask.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/445730488/StdTask.p1 ../Source/Standard/Timer/StdTask.c 
	@-${MV} ${OBJECTDIR}/_ext/445730488/StdTask.d ${OBJECTDIR}/_ext/445730488/StdTask.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/445730488/StdTask.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2038438061/StdDS2485.p1: ../Source/Standard/I2C/StdDS2485.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2038438061" 
	@${RM} ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1 ../Source/Standard/I2C/StdDS2485.c 
	@-${MV} ${OBJECTDIR}/_ext/2038438061/StdDS2485.d ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2038438061/StdDS2485.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/962826093/System.p1: ../Source/System/PIC18F66K80/System.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/962826093" 
	@${RM} ${OBJECTDIR}/_ext/962826093/System.p1.d 
	@${RM} ${OBJECTDIR}/_ext/962826093/System.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/962826093/System.p1 ../Source/System/PIC18F66K80/System.c 
	@-${MV} ${OBJECTDIR}/_ext/962826093/System.d ${OBJECTDIR}/_ext/962826093/System.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/962826093/System.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppIrq.p1: AppIrq.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppIrq.p1.d 
	@${RM} ${OBJECTDIR}/AppIrq.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppIrq.p1 AppIrq.c 
	@-${MV} ${OBJECTDIR}/AppIrq.d ${OBJECTDIR}/AppIrq.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppIrq.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppZCD.p1: AppZCD.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppZCD.p1.d 
	@${RM} ${OBJECTDIR}/AppZCD.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppZCD.p1 AppZCD.c 
	@-${MV} ${OBJECTDIR}/AppZCD.d ${OBJECTDIR}/AppZCD.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppZCD.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppButton.p1: AppButton.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppButton.p1.d 
	@${RM} ${OBJECTDIR}/AppButton.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppButton.p1 AppButton.c 
	@-${MV} ${OBJECTDIR}/AppButton.d ${OBJECTDIR}/AppButton.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppButton.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppCan.p1: AppCan.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppCan.p1.d 
	@${RM} ${OBJECTDIR}/AppCan.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppCan.p1 AppCan.c 
	@-${MV} ${OBJECTDIR}/AppCan.d ${OBJECTDIR}/AppCan.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppCan.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppDimmer.p1: AppDimmer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppDimmer.p1.d 
	@${RM} ${OBJECTDIR}/AppDimmer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppDimmer.p1 AppDimmer.c 
	@-${MV} ${OBJECTDIR}/AppDimmer.d ${OBJECTDIR}/AppDimmer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppDimmer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppEeprom.p1: AppEeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppEeprom.p1.d 
	@${RM} ${OBJECTDIR}/AppEeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppEeprom.p1 AppEeprom.c 
	@-${MV} ${OBJECTDIR}/AppEeprom.d ${OBJECTDIR}/AppEeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppEeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppLed.p1: AppLed.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppLed.p1.d 
	@${RM} ${OBJECTDIR}/AppLed.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppLed.p1 AppLed.c 
	@-${MV} ${OBJECTDIR}/AppLed.d ${OBJECTDIR}/AppLed.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppLed.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppAction.p1: AppAction.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppAction.p1.d 
	@${RM} ${OBJECTDIR}/AppAction.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppAction.p1 AppAction.c 
	@-${MV} ${OBJECTDIR}/AppAction.d ${OBJECTDIR}/AppAction.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppAction.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppScenarios.p1: AppScenarios.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppScenarios.p1.d 
	@${RM} ${OBJECTDIR}/AppScenarios.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppScenarios.p1 AppScenarios.c 
	@-${MV} ${OBJECTDIR}/AppScenarios.d ${OBJECTDIR}/AppScenarios.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppScenarios.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppSfeer.p1: AppSfeer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppSfeer.p1.d 
	@${RM} ${OBJECTDIR}/AppSfeer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppSfeer.p1 AppSfeer.c 
	@-${MV} ${OBJECTDIR}/AppSfeer.d ${OBJECTDIR}/AppSfeer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppSfeer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppService.p1: AppService.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppService.p1.d 
	@${RM} ${OBJECTDIR}/AppService.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppService.p1 AppService.c 
	@-${MV} ${OBJECTDIR}/AppService.d ${OBJECTDIR}/AppService.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppService.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/AppRTC.p1: AppRTC.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AppRTC.p1.d 
	@${RM} ${OBJECTDIR}/AppRTC.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/AppRTC.p1 AppRTC.c 
	@-${MV} ${OBJECTDIR}/AppRTC.d ${OBJECTDIR}/AppRTC.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/AppRTC.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/FerDimBasic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/FerDimBasic.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=pickit3  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/FerDimBasic.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/FerDimBasic.X.${IMAGE_TYPE}.hex 
	
else
${DISTDIR}/FerDimBasic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/FerDimBasic.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/FerDimBasic.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
