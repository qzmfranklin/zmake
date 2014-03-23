################################################################################
#  A set of template files to build projects with a single Makefile
#  Copyright (C) 2013-2014 Zhongming Qu <qzmfrank@umich.edu>
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
#  THIS DIRECTORY
DIR0001:=${ROOT}/heapsort
#  ALL C/C++ FILES IN THIS DIR0001ECTORY (WITHOUT PATHNAME)
${DIR0001}C:=
${DIR0001}CPP:=heapsort.cpp test_heapsort.cpp
#  DIR0001ECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIR0001ECTORIES
${DIR0001}CFLAGS:=${CFLAGS}
${DIR0001}CXXFLAGS:=${CXXFLAGS}
${DIR0001}INCS:=${INCS}
################################################################################ 
#		INTERNAL VARIABLES AND PATTERN RULES
# in-place build
# Process variables
${DIR0001}DEP:=${${DIR0001}CPP:%.cpp=${DIR0001}/%.d} ${${DIR0001}C:%.c=${DIR0001}/%.d} 
${DIR0001}OBJ:=${${DIR0001}CPP:%.cpp=${DIR0001}/%.o} ${${DIR0001}C:%.c=${DIR0001}/%.o} 
${DIR0001}ASM:=${${DIR0001}CPP:%.cpp=${DIR0001}/%.s} ${${DIR0001}C:%.c=${DIR0001}/%.s} 
# Add to global variables
DEP:=${DEP} ${${DIR0001}DEP}
OBJ:=${OBJ} ${${DIR0001}OBJ}
ASM:=${ASM} ${${DIR0001}ASM}

# C sources
${DIR0001}/%.o: ${DIR0001}/%.c
	@echo Compiling "${GREEN}$@${NONE}"...
	${QUIET}${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR0001}CFLAGS} ${${DIR0001}INCS}
${DIR0001}/%.s: ${DIR0001}/%.c
	@echo Compiling "${CYAN}$@${NONE}"...
	${QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR0001}CFLAGS} ${${DIR0001}INCS}

# C++ sources
${DIR0001}/%.o: ${DIR0001}/%.cpp
	@echo Generating "${GREEN}$@${NONE}"...
	${QUIET}${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR0001}CXXFLAGS} ${${DIR0001}INCS}
${DIR0001}/%.s: ${DIR0001}/%.cpp
	@echo Generating "${CYAN}$@${NONE}"...
	${QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR0001}CXXFLAGS} ${${DIR0001}INCS}


# out-of-source build
#${DIR0001}DEP:=${${DIR0001}CPP:%.cpp=${BUILD}/%.d} ${${DIR0001}C:%.c=${BUILD}/%.d} 
#${DIR0001}OBJ:=${${DIR0001}CPP:%.cpp=${BUILD}/%.o} ${${DIR0001}C:%.c=${BUILD}/%.o} 
#${DIR0001}ASM:=${${DIR0001}CPP:%.cpp=${BUILD}/%.s} ${${DIR0001}C:%.c=${BUILD}/%.s} 
## Add to global variables
#DEP:=${DEP} ${${DIR0001}DEP}
#OBJ:=${OBJ} ${${DIR0001}OBJ}
#ASM:=${ASM} ${${DIR0001}ASM}
## C sources
#${BUILD}/%.o: ${DIR0001}/%.c
	#@echo Generating "${GREEN}$@${NONE}"...
	#${QUIET}${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR0001}CFLAGS} ${${DIR0001}INCS}
#${BUILD}/%.s: ${DIR0001}/%.c
	#@echo Generating "${CYAN}$@${NONE}"...
	#${QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR0001}CFLAGS} ${${DIR0001}INCS}

## C++ sources
#${BUILD}/%.o: ${DIR0001}/%.cpp
	#@echo Generating "${GREEN}$@${NONE}"...
	#${QUIET}${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR0001}CXXFLAGS} ${${DIR0001}INCS}
#${BUILD}/%.s: ${DIR0001}/%.cpp
	#@echo Generating "${CYAN}$@${NONE}"...
	#${QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR0001}CXXFLAGS} ${${DIR0001}INCS}
