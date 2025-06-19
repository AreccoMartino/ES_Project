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
${OBJECTDIR}/general_purpose_lib.o: general_purpose_lib.c  .generated_files/flags/default/1b374c0ca826d4af4b2b001fc335f2c08ff087bc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/general_purpose_lib.o.d 
	@${RM} ${OBJECTDIR}/general_purpose_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  general_purpose_lib.c  -o ${OBJECTDIR}/general_purpose_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/general_purpose_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/timer_lib.o: timer_lib.c  .generated_files/flags/default/a30f827ce27c17474b9840d4dc33abbb1880ea5a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer_lib.o.d 
	@${RM} ${OBJECTDIR}/timer_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timer_lib.c  -o ${OBJECTDIR}/timer_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/timer_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/uart_lib.o: uart_lib.c  .generated_files/flags/default/e0835a937790705bc45cc35590765ce1ec38709e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart_lib.o.d 
	@${RM} ${OBJECTDIR}/uart_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uart_lib.c  -o ${OBJECTDIR}/uart_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/uart_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/spi_lib.o: spi_lib.c  .generated_files/flags/default/9e7fed49f23ed22bf04c4ab0a85a2988f97dd723 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/spi_lib.o.d 
	@${RM} ${OBJECTDIR}/spi_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  spi_lib.c  -o ${OBJECTDIR}/spi_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/spi_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/parser_lib.o: parser_lib.c  .generated_files/flags/default/1a7e989cad04fe56d5e6c8f87116b4f50296fb2b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/parser_lib.o.d 
	@${RM} ${OBJECTDIR}/parser_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  parser_lib.c  -o ${OBJECTDIR}/parser_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/parser_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pwm_lib.o: pwm_lib.c  .generated_files/flags/default/33efd43183f461c22b7fdd424e2aea0eb6bc538 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm_lib.o.d 
	@${RM} ${OBJECTDIR}/pwm_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pwm_lib.c  -o ${OBJECTDIR}/pwm_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pwm_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/adc_lib.o: adc_lib.c  .generated_files/flags/default/6673d83f2d6673b12157b5737407cf01c3029d5e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc_lib.o.d 
	@${RM} ${OBJECTDIR}/adc_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  adc_lib.c  -o ${OBJECTDIR}/adc_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/adc_lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/general_purpose_lib.o: general_purpose_lib.c  .generated_files/flags/default/a6507b42c2730f46246cb6eec630c6fbf52aebb4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/general_purpose_lib.o.d 
	@${RM} ${OBJECTDIR}/general_purpose_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  general_purpose_lib.c  -o ${OBJECTDIR}/general_purpose_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/general_purpose_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/timer_lib.o: timer_lib.c  .generated_files/flags/default/930d313ef22ccf3ccc7dfdaac0c7377f17d76913 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer_lib.o.d 
	@${RM} ${OBJECTDIR}/timer_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timer_lib.c  -o ${OBJECTDIR}/timer_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/timer_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/uart_lib.o: uart_lib.c  .generated_files/flags/default/65dfb690888fc798b0d4f1188a999b672d39a5c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart_lib.o.d 
	@${RM} ${OBJECTDIR}/uart_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uart_lib.c  -o ${OBJECTDIR}/uart_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/uart_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/spi_lib.o: spi_lib.c  .generated_files/flags/default/82ca6d176e2ff12b508fca5ca6c211d8cc7fffe2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/spi_lib.o.d 
	@${RM} ${OBJECTDIR}/spi_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  spi_lib.c  -o ${OBJECTDIR}/spi_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/spi_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/parser_lib.o: parser_lib.c  .generated_files/flags/default/c571e94ac2ef836707bec453d2ab59a2ee577e22 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/parser_lib.o.d 
	@${RM} ${OBJECTDIR}/parser_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  parser_lib.c  -o ${OBJECTDIR}/parser_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/parser_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pwm_lib.o: pwm_lib.c  .generated_files/flags/default/29fa28701e8a37d758769c8258a42edb81389010 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm_lib.o.d 
	@${RM} ${OBJECTDIR}/pwm_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pwm_lib.c  -o ${OBJECTDIR}/pwm_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pwm_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/adc_lib.o: adc_lib.c  .generated_files/flags/default/d1bd774352e1408032cdc98f4d20dad917b82497 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc_lib.o.d 
	@${RM} ${OBJECTDIR}/adc_lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  adc_lib.c  -o ${OBJECTDIR}/adc_lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/adc_lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -msmall-data -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
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
