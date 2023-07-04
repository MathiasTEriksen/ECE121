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
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/lab2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/lab2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=BOARD.c uart.c FreeRunningTimer.c Protocol2.c RotaryEncoder.c RCServo.c PingSensor.c Lab2Application.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/BOARD.o ${OBJECTDIR}/uart.o ${OBJECTDIR}/FreeRunningTimer.o ${OBJECTDIR}/Protocol2.o ${OBJECTDIR}/RotaryEncoder.o ${OBJECTDIR}/RCServo.o ${OBJECTDIR}/PingSensor.o ${OBJECTDIR}/Lab2Application.o
POSSIBLE_DEPFILES=${OBJECTDIR}/BOARD.o.d ${OBJECTDIR}/uart.o.d ${OBJECTDIR}/FreeRunningTimer.o.d ${OBJECTDIR}/Protocol2.o.d ${OBJECTDIR}/RotaryEncoder.o.d ${OBJECTDIR}/RCServo.o.d ${OBJECTDIR}/PingSensor.o.d ${OBJECTDIR}/Lab2Application.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/BOARD.o ${OBJECTDIR}/uart.o ${OBJECTDIR}/FreeRunningTimer.o ${OBJECTDIR}/Protocol2.o ${OBJECTDIR}/RotaryEncoder.o ${OBJECTDIR}/RCServo.o ${OBJECTDIR}/PingSensor.o ${OBJECTDIR}/Lab2Application.o

# Source Files
SOURCEFILES=BOARD.c uart.c FreeRunningTimer.c Protocol2.c RotaryEncoder.c RCServo.c PingSensor.c Lab2Application.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/lab2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX340F512H
MP_LINKER_FILE_OPTION=
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
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/BOARD.o: BOARD.c  .generated_files/flags/default/22762510198cede0140ce1c1992b01f457a615b3 .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BOARD.o.d 
	@${RM} ${OBJECTDIR}/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/BOARD.o.d" -o ${OBJECTDIR}/BOARD.o BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/uart.o: uart.c  .generated_files/flags/default/1e52c565caa1a494b595927aae99ded56720a14 .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart.o.d 
	@${RM} ${OBJECTDIR}/uart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/uart.o.d" -o ${OBJECTDIR}/uart.o uart.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/FreeRunningTimer.o: FreeRunningTimer.c  .generated_files/flags/default/dc57378df5ba07d8f3f843b99a79aafbe4fabede .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/FreeRunningTimer.o.d 
	@${RM} ${OBJECTDIR}/FreeRunningTimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/FreeRunningTimer.o.d" -o ${OBJECTDIR}/FreeRunningTimer.o FreeRunningTimer.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Protocol2.o: Protocol2.c  .generated_files/flags/default/11ce8b2bb2835c3dd7af596647b5f968a0bf2360 .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Protocol2.o.d 
	@${RM} ${OBJECTDIR}/Protocol2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/Protocol2.o.d" -o ${OBJECTDIR}/Protocol2.o Protocol2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/RotaryEncoder.o: RotaryEncoder.c  .generated_files/flags/default/f013e40afd4007e59cfda42b8a6803a05f0f91d6 .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RotaryEncoder.o.d 
	@${RM} ${OBJECTDIR}/RotaryEncoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/RotaryEncoder.o.d" -o ${OBJECTDIR}/RotaryEncoder.o RotaryEncoder.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/RCServo.o: RCServo.c  .generated_files/flags/default/be8360a04d084279e9d61b3236b75fda1fd3e28f .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RCServo.o.d 
	@${RM} ${OBJECTDIR}/RCServo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/RCServo.o.d" -o ${OBJECTDIR}/RCServo.o RCServo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/PingSensor.o: PingSensor.c  .generated_files/flags/default/a479ea86fb8f16ce311362f95ce3841efee65b79 .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PingSensor.o.d 
	@${RM} ${OBJECTDIR}/PingSensor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/PingSensor.o.d" -o ${OBJECTDIR}/PingSensor.o PingSensor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Lab2Application.o: Lab2Application.c  .generated_files/flags/default/76503015499e470d174386b6579ff7876360f60d .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Lab2Application.o.d 
	@${RM} ${OBJECTDIR}/Lab2Application.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/Lab2Application.o.d" -o ${OBJECTDIR}/Lab2Application.o Lab2Application.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/BOARD.o: BOARD.c  .generated_files/flags/default/b3208c6b024e3d942e487160f58d6a0257c78263 .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BOARD.o.d 
	@${RM} ${OBJECTDIR}/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/BOARD.o.d" -o ${OBJECTDIR}/BOARD.o BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/uart.o: uart.c  .generated_files/flags/default/8204c45cfa4df91fdd2f26d9525fa7d6fe36c75f .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart.o.d 
	@${RM} ${OBJECTDIR}/uart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/uart.o.d" -o ${OBJECTDIR}/uart.o uart.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/FreeRunningTimer.o: FreeRunningTimer.c  .generated_files/flags/default/98910e0862993fa46dab1d53f669e6c47a8f6175 .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/FreeRunningTimer.o.d 
	@${RM} ${OBJECTDIR}/FreeRunningTimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/FreeRunningTimer.o.d" -o ${OBJECTDIR}/FreeRunningTimer.o FreeRunningTimer.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Protocol2.o: Protocol2.c  .generated_files/flags/default/e378f2b1a73f6d24a7d6514e877df4cdb4856d50 .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Protocol2.o.d 
	@${RM} ${OBJECTDIR}/Protocol2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/Protocol2.o.d" -o ${OBJECTDIR}/Protocol2.o Protocol2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/RotaryEncoder.o: RotaryEncoder.c  .generated_files/flags/default/8d369e4a65507d771a42e58c461ec802f42641e7 .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RotaryEncoder.o.d 
	@${RM} ${OBJECTDIR}/RotaryEncoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/RotaryEncoder.o.d" -o ${OBJECTDIR}/RotaryEncoder.o RotaryEncoder.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/RCServo.o: RCServo.c  .generated_files/flags/default/cadf0244c48849861cde928cbf6c7ea908881918 .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RCServo.o.d 
	@${RM} ${OBJECTDIR}/RCServo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/RCServo.o.d" -o ${OBJECTDIR}/RCServo.o RCServo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/PingSensor.o: PingSensor.c  .generated_files/flags/default/4cfcb3674725ee078a1e103c58a0abeed3ec42a3 .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PingSensor.o.d 
	@${RM} ${OBJECTDIR}/PingSensor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/PingSensor.o.d" -o ${OBJECTDIR}/PingSensor.o PingSensor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Lab2Application.o: Lab2Application.c  .generated_files/flags/default/279ae1b96e0515628ef0943f33e82f2d068a8178 .generated_files/flags/default/b049a9946035c0d6f8c1361b9c30a33141c1b72d
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Lab2Application.o.d 
	@${RM} ${OBJECTDIR}/Lab2Application.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -MP -MMD -MF "${OBJECTDIR}/Lab2Application.o.d" -o ${OBJECTDIR}/Lab2Application.o Lab2Application.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/lab2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/lab2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=4096,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/lab2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/lab2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=4096,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}/xc32-bin2hex ${DISTDIR}/lab2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
