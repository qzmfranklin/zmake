################################################################################
#  A set of template files to build projects with a single Makefile
#  Copyright (C) 2013 Zhongming Qu <qzmfrank@umich.edu>
#  
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
################################################################################
#				STEP 1
#	DIRECTORY NAME FROM THE ROOT DIRECTORY
DIR005:=${SRC}/itrsol# Please, NO SPACE 
# Quiet build for this dirctory. Comment the following line to be verbose.
${DIR005}QUIET:=@
###############################################################################
#				STEP 2
#	DIRECTORY-SPECIFIC COMPILING AND LINKING OPTIONS
${DIR005}CXXFLAGS:=${CXXFLAGS}
${DIR005}INCS:=${INCS}
###############################################################################
#				STEP 3
#	DIRECTORY-SPECIFIC SOURCE FILES
${DIR005}CPPFILES:=
###############################################################################
#				STEP 4
#	DIRECTORY-SPECIFIC BINARY OUTPUTS: EXECUTABLES and LIBRARIES
${DIR005}BINEXE:=
###############################################################################
#				STEP 5
#	DIRECTORY-SPECIFIC TEST FILES
${DIR005}TSTEXE:= test_mkl_solvers.exe

${BIN}/test_mkl_solvers.exe: ${OBJ}/test_mkl_solvers.o ${OBJ}/utils.o
###############################################################################
#	Congratulations! You have completed everything you need to do to build
#  this directory. You do not need to modify this file unless some C and/or
#  CPP source files are added, removed, or renamed. In that case, just go over
#  STEP 1-5 again and it is done!
###############################################################################


































###############################################################################
###############################################################################
###############################################################################
###############################################################################
#	The following four blocks of codes generate proper directory-specific
#  variables and add proper files to the top level variables. Please anyway do
#  NOT modify them.
################## DO NOT MODIFY ################
${DIR005}CPPFILES:=${${DIR005}CPPFILES:%=${DIR005}/%}
${DIR005}OBJFILES:=${${DIR005}CPPFILES:${DIR005}%.cpp=${OBJ}%.o}
${DIR005}DEPFILES:=${${DIR005}OBJFILES:%.o=%.d}
${DIR005}ASMFILES:=${${DIR005}OBJFILES:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR005}BINEXE:=${${DIR005}BINEXE:%=${BIN}/%}
${DIR005}BINCPP:=${${DIR005}BINEXE:${BIN}/%.exe=${DIR005}/%.cpp}
${DIR005}BINOBJ:=${${DIR005}BINCPP:${DIR005}%.cpp=${OBJ}%.o}
${DIR005}BINDEP:=${${DIR005}BINOBJ:%.o=%.d}
${DIR005}BINASM:=${${DIR005}BINOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
${DIR005}TSTEXE:=${${DIR005}TSTEXE:%=${BIN}/%}
${DIR005}TSTCPP:=${${DIR005}TSTEXE:${BIN}/%.exe=${DIR005}/%.cpp}
${DIR005}TSTOBJ:=${${DIR005}TSTCPP:${DIR005}%.cpp=${OBJ}%.o}
${DIR005}TSTDEP:=${${DIR005}TSTOBJ:%.o=%.d}
${DIR005}TSTASM:=${${DIR005}TSTOBJ:${OBJ}%.o=${ASM}%.s}
################## DO NOT MODIFY ################
SRCFILES:=${SRCFILES} ${${DIR005}CFILES} \
	${${DIR005}CPPFILES} ${${DIR005}TSTCPP} \
	${${DIR005}BINCPP}
DEPFILES:=${DEPFILES} ${${DIR005}DEPFILES} \
	${${DIR005}TSTDEP} ${${DIR005}BINDEP}
################## DO NOT MODIFY ################
###############################################################################
#				WANRING
#  Please do not modify the following part if you are not 100% sure about it.
#  The design of this file is that one only needs to modify the part above.
#  If you find you need to modify any of the parts below, be very careful!
#				WANRING
###############################################################################
###############################################################################

#DIRECTORY-SPECIFIC PATTERN RULES
#
#	C++ linkage at the top level is done in the Makefile.vars in the root 
#  directory. Any special build rules for libraries and executables should be
#  secified in STEP 6 in this makevars.mk file.
#  	Not using "\" to break long lines because that would hurt the non-quiet 
#  mode output aestetics. The new colorful version of "make list" is ready now!

#DIRECTORY-SPECIFIC PHONY TARGETS
.PHONY: ${DIR005}-all ${DIR005}-test \
	${DIR005}-asm ${DIR005}-check \
	${DIR005}-list

#  C++ sources
${OBJ}/%.o: ${DIR005}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"...
	${${DIR005}QUIET}${CXX} -o $@ -c $< ${${DIR005}CXXFLAGS} ${${DIR005}INCS}
${ASM}/%.s: ${DIR005}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${${DIR005}QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR005}CXXFLAGS} ${${DIR005}INCS} 

	
TARGET_ALL	:=${TARGET_ALL} ${DIR005}-all
TARGET_TEST	:=${TARGET_TEST} ${DIR005}-test
TARGET_ASM	:=${TARGET_ASM} ${DIR005}-asm
TARGET_CHECK	:=${TARGET_CHECK} ${DIR005}-check
TARGET_LIST	:=${TARGET_LIST} ${DIR005}-list
${DIR005}-all: ${${DIR005}OBJFILES} ${${DIR005}BINEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".
${DIR005}-test: ${${DIR005}TSTEXE}
	@echo Finished building "${B_BLUE}$@${NONE}".  
${DIR005}-asm: ${${DIR005}ASMFILES} ${${DIR005}TSTASM} \
	${${DIR005}BINASM}
	@echo Finished generating "${B_BLUE}$@${NONE}".  
${DIR005}-check: ${${DIR005}TSTEXE:${BIN}/%.exe=${OUTPUT}/%.txt}
${DIR005}-list:
	@echo \#\#\#\#\#\#\#\#"${B_BROWN}$@${NONE}"\#\#\#\#\#\#\#\#
	@$(foreach dir, 						\
		CFILES CPPFILES TSTCPP BINCPP				\
		,							\
		if [ ! -z "${${DIR005}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR005}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${BLACK}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		TSTEXE BINEXE						\
		,							\
		if [ ! -z "${${DIR005}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR005}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${RED}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;)
	@$(foreach dir, 						\
		OBJFILES TSTOBJ BINOBJ					\
		,							\
		if [ ! -z "${${DIR005}${dir}}" ]; then 		\
			echo "${BROWN}${dir}${NONE}\t\c";		\
			$(foreach file,${${DIR005}${dir}},		\
			    if [ -f ${file} ]; then echo		\
				"${GREEN}${file}${NONE}\c";		\
			    else echo "${GREY}${file}${NONE}\c";	\
			    fi;)					\
			echo;						\
		fi;) 
