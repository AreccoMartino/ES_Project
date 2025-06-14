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
OUTPUT_SUFFIX=a
DEBUGGABLE_SUFFIX=a
FINAL_IMAGE=${DISTDIR}/LIBRARY.X.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=a
DEBUGGABLE_SUFFIX=a
FINAL_IMAGE=${DISTDIR}/LIBRARY.X.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=general_purpose_lib.c timer_lib.c uart_lib.c spi_lib.c parser_lib.c pwm_lib.c adc_lib.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/general_purpose_lib.o ${OBJECTDIR}/timer_lib.o ${OBJECTDIR}/uart_lib.o ${OBJECTDIR}/spi_lib.o ${OBJECTDIR}/parser_lib.o ${OBJECTDIR}/pwm_lib.o ${OBJECTDIR}/adc_lib.o
POSSIBLE_DEPFILES=${OBJECTDIR}/general_purpose_lib.o.d ${OBJECTDIR}/timer_lib.o.d ${OBJECTDIR}/uart_lib.o.d ${OBJECTDIR}/spi_lib.o.d ${OBJECTDIR}/parser_lib.o.d ${OBJECTDIR}/pwm_lib.o.d ${OBJECTDIR}/adc_lib.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/general_purpose_lib.o ${OBJECTDIR}/timer_lib.o ${OBJECTDIR}/uart_lib.o ${OBJECTDIR}/spi_lib.o ${OBJECTDIR}/parser_lib.o ${OBJECTDIR}/pwm_lib.o ${OBJECTDIR}/adc_lib.o

# Source Files
SOURCEFILES=general_purpose_lib.c timer_lib.c uart_lib.c spi_lib.c parser_lib.c pwm_lib.c adc_lib.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/LIBRARY.X.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512MU810
MP_LINKER_FILE_OPTION=,--script=p33EP512MU810.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/general_purpose_lib.o: general_purpose_lib.c  .generated_files/flags/default/bd11e43de995e0a9fe9083b7825627884c2db78e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/general_purpose_lib.o.d 
	@${RM} ${OBJECTDIR}/general_purpose_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  general_purpose_lib.c  -o ${OBJECTDIR}/general_purpose_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/general_purpose_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/timer_lib.o: timer_lib.c  .generated_files/flags/default/ea6b9784f6318070a5c87c3d5ff9f8d1ab649d0e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer_lib.o.d 
	@${RM} ${OBJECTDIR}/timer_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timer_lib.c  -o ${OBJECTDIR}/timer_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/timer_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/uart_lib.o: uart_lib.c  .generated_files/flags/default/430a8de332a7205fd93c7f909135e0e76054c5b2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart_lib.o.d 
	@${RM} ${OBJECTDIR}/uart_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uart_lib.c  -o ${OBJECTDIR}/uart_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/uart_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/spi_lib.o: spi_lib.c  .generated_files/flags/default/77baa13bea2ac290e6a842c6c767763c2e6122cf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/spi_lib.o.d 
	@${RM} ${OBJECTDIR}/spi_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  spi_lib.c  -o ${OBJECTDIR}/spi_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/spi_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/parser_lib.o: parser_lib.c  .generated_files/flags/default/bb2be0677c58b2d81fb117ac52e7fd3813de9496 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/parser_lib.o.d 
	@${RM} ${OBJECTDIR}/parser_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  parser_lib.c  -o ${OBJECTDIR}/parser_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/parser_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pwm_lib.o: pwm_lib.c  .generated_files/flags/default/cc49d8d6500d365d0999b57705888ef519c84603 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm_lib.o.d 
	@${RM} ${OBJECTDIR}/pwm_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pwm_lib.c  -o ${OBJECTDIR}/pwm_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pwm_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/adc_lib.o: adc_lib.c  .generated_files/flags/default/ca3bc77a0e5d609df46b9f796bd0402a0e5e0c15 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc_lib.o.d 
	@${RM} ${OBJECTDIR}/adc_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  adc_lib.c  -o ${OBJECTDIR}/adc_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/adc_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/general_purpose_lib.o: general_purpose_lib.c  .generated_files/flags/default/3e8a634094dc241b732e670960614f6a032593e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/general_purpose_lib.o.d 
	@${RM} ${OBJECTDIR}/general_purpose_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  general_purpose_lib.c  -o ${OBJECTDIR}/general_purpose_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/general_purpose_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/timer_lib.o: timer_lib.c  .generated_files/flags/default/9daf0653e4125026352badeff11aa872176e09e8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer_lib.o.d 
	@${RM} ${OBJECTDIR}/timer_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timer_lib.c  -o ${OBJECTDIR}/timer_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/timer_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/uart_lib.o: uart_lib.c  .generated_files/flags/default/74c6625b8a22afa60834e09d4d7335f80e081bb0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart_lib.o.d 
	@${RM} ${OBJECTDIR}/uart_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uart_lib.c  -o ${OBJECTDIR}/uart_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/uart_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/spi_lib.o: spi_lib.c  .generated_files/flags/default/eef2f13de2b304a870d5c7dc6560ce195cbda6b5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/spi_lib.o.d 
	@${RM} ${OBJECTDIR}/spi_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  spi_lib.c  -o ${OBJECTDIR}/spi_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/spi_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/parser_lib.o: parser_lib.c  .generated_files/flags/default/ad1a1f85fbad224827ca82378cc82d2f038d715 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/parser_lib.o.d 
	@${RM} ${OBJECTDIR}/parser_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  parser_lib.c  -o ${OBJECTDIR}/parser_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/parser_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pwm_lib.o: pwm_lib.c  .generated_files/flags/default/dadddbce398d915534618f37aa4047f88531632f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm_lib.o.d 
	@${RM} ${OBJECTDIR}/pwm_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pwm_lib.c  -o ${OBJECTDIR}/pwm_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pwm_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/adc_lib.o: adc_lib.c  .generated_files/flags/default/e94201aafb86e1805031ca1ec5ac6de8bf4de403 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc_lib.o.d 
	@${RM} ${OBJECTDIR}/adc_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  adc_lib.c  -o ${OBJECTDIR}/adc_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/adc_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: archive
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/LIBRARY.X.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	@${RM} ${DISTDIR}/LIBRARY.X.${OUTPUT_SUFFIX} 
	${MP_AR} $(MP_EXTRA_AR_PRE)  -omf=elf -r ${DISTDIR}/LIBRARY.X.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}       -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/LIBRARY.X.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	@${RM} ${DISTDIR}/LIBRARY.X.${OUTPUT_SUFFIX} 
	${MP_AR} $(MP_EXTRA_AR_PRE)  -omf=elf -r ${DISTDIR}/LIBRARY.X.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}       -mdfp="${DFP_DIR}/xc16" 
	
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

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
