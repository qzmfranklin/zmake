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
DIR0002:=${ROOT}/diffpar
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR0002}C:=diffpar.c
${DIR0002}CPP:=
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR0002}CFLAGS:=${CFLAGS}
${DIR0002}CXXFLAGS:=${CXXFLAGS}
${DIR0002}INCS:=${INCS}
################################################################################ 
#		INTERNAL VARIABLES AND PATTERN RULES
ifeq (${OUT_OF_SOURCE},TRUE)
# out-of-source build
# Process variables
${DIR0002}DEP:=${${DIR0002}CPP:%.cpp=${BUILD}%.d} ${${DIR0002}C:%.c=${BUILD}%.d} 
${DIR0002}OBJ:=${${DIR0002}CPP:%.cpp=${BUILD}%.o} ${${DIR0002}C:%.c=${BUILD}%.o} 
${DIR0002}ASM:=${${DIR0002}CPP:%.cpp=${BUILD}%.s} ${${DIR0002}C:%.c=${BUILD}%.s} 
# Add to global variables
DEP:=${DEP} ${${DIR0002}DEP}
OBJ:=${OBJ} ${${DIR0002}OBJ}
ASM:=${ASM} ${${DIR0002}ASM}
# C sources
${BUILD}%.o: ${DIR0002}/%.c
	@echo Compiling "${GREEN}$@${NONE}"
	${QUIET}${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR0002}CFLAGS} ${${DIR0002}INCS}
${BUILD}%.s: ${DIR0002}/%.c
	@echo Compiling "${CYAN}$@${NONE}"
	${QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR0002}CFLAGS} ${${DIR0002}INCS}

# C++ sources
${BUILD}%.o: ${DIR0002}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"
	${QUIET}${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR0002}CXXFLAGS} ${${DIR0002}INCS}
${BUILD}%.s: ${DIR0002}/%.cpp
	@echo Compiling "${CYAN}$@${NONE}"
	${QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR0002}CXXFLAGS} ${${DIR0002}INCS}
else
# in-place build
# Process variables
${DIR0002}DEP:=${${DIR0002}CPP:%.cpp=${DIR0002}/%.d} ${${DIR0002}C:%.c=${DIR0002}/%.d} 
${DIR0002}OBJ:=${${DIR0002}CPP:%.cpp=${DIR0002}/%.o} ${${DIR0002}C:%.c=${DIR0002}/%.o} 
${DIR0002}ASM:=${${DIR0002}CPP:%.cpp=${DIR0002}/%.s} ${${DIR0002}C:%.c=${DIR0002}/%.s} 
# Add to global variables
DEP:=${DEP} ${${DIR0002}DEP}
OBJ:=${OBJ} ${${DIR0002}OBJ}
ASM:=${ASM} ${${DIR0002}ASM}

# C sources
${DIR0002}/%.o: ${DIR0002}/%.c
	@echo Compiling "${GREEN}$@${NONE}"
	${QUIET}${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR0002}CFLAGS} ${${DIR0002}INCS}
${DIR0002}/%.s: ${DIR0002}/%.c
	@echo Compiling "${CYAN}$@${NONE}"
	${QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR0002}CFLAGS} ${${DIR0002}INCS}

# C++ sources
${DIR0002}/%.o: ${DIR0002}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"
	${QUIET}${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR0002}CXXFLAGS} ${${DIR0002}INCS}
${DIR0002}/%.s: ${DIR0002}/%.cpp
	@echo Compiling "${CYAN}$@${NONE}"
	${QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR0002}CXXFLAGS} ${${DIR0002}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR0002}/%.o
	@echo Linking "${RED}$@${NONE}"
	${QUIET}${CXX} -o $@ $^ ${LIBS}
endif 
