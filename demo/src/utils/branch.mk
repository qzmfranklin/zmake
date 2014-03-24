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
DIR0003:=${ROOT}/utils
#  ALL C/C++ FILES IN THIS DIRECTORY (WITHOUT PATHNAME)
${DIR0003}C:=
${DIR0003}CPP:=utils.cpp
#  DIRECTORY-SPECIFIC COMPILING FLAGS AND INCLUDE DIRECTORIES
${DIR0003}CFLAGS:=${CFLAGS}
${DIR0003}CXXFLAGS:=${CXXFLAGS}
${DIR0003}INCS:=${INCS}
################################################################################ 
#		INTERNAL VARIABLES AND PATTERN RULES
ifeq (${OUT_OF_SOURCE},TRUE)
# out-of-source build
# Process variables
${DIR0003}DEP:=${${DIR0003}CPP:%.cpp=${BUILD}/%.d} ${${DIR0003}C:%.c=${BUILD}/%.d} 
${DIR0003}OBJ:=${${DIR0003}CPP:%.cpp=${BUILD}/%.o} ${${DIR0003}C:%.c=${BUILD}/%.o} 
${DIR0003}ASM:=${${DIR0003}CPP:%.cpp=${BUILD}/%.s} ${${DIR0003}C:%.c=${BUILD}/%.s} 
# Add to global variables
DEP:=${DEP} ${${DIR0003}DEP}
OBJ:=${OBJ} ${${DIR0003}OBJ}
ASM:=${ASM} ${${DIR0003}ASM}
# C sources
${BUILD}/%.o: ${DIR0003}/%.c
	@echo Compiling "${GREEN}$@${NONE}"
	${QUIET}${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR0003}CFLAGS} ${${DIR0003}INCS}
${BUILD}/%.s: ${DIR0003}/%.c
	@echo Compiling "${CYAN}$@${NONE}"
	${QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR0003}CFLAGS} ${${DIR0003}INCS}

# C++ sources
${BUILD}/%.o: ${DIR0003}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"
	${QUIET}${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR0003}CXXFLAGS} ${${DIR0003}INCS}
${BUILD}/%.s: ${DIR0003}/%.cpp
	@echo Compiling "${CYAN}$@${NONE}"
	${QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR0003}CXXFLAGS} ${${DIR0003}INCS}
else
# in-place build
# Process variables
${DIR0003}DEP:=${${DIR0003}CPP:%.cpp=${DIR0003}/%.d} ${${DIR0003}C:%.c=${DIR0003}/%.d} 
${DIR0003}OBJ:=${${DIR0003}CPP:%.cpp=${DIR0003}/%.o} ${${DIR0003}C:%.c=${DIR0003}/%.o} 
${DIR0003}ASM:=${${DIR0003}CPP:%.cpp=${DIR0003}/%.s} ${${DIR0003}C:%.c=${DIR0003}/%.s} 
# Add to global variables
DEP:=${DEP} ${${DIR0003}DEP}
OBJ:=${OBJ} ${${DIR0003}OBJ}
ASM:=${ASM} ${${DIR0003}ASM}

# C sources
${DIR0003}/%.o: ${DIR0003}/%.c
	@echo Compiling "${GREEN}$@${NONE}"
	${QUIET}${CC} -o $@ -c $< ${DEPFLAGS} ${${DIR0003}CFLAGS} ${${DIR0003}INCS}
${DIR0003}/%.s: ${DIR0003}/%.c
	@echo Compiling "${CYAN}$@${NONE}"
	${QUIET}${CC} -o $@ $< ${ASMFLAGS} ${${DIR0003}CFLAGS} ${${DIR0003}INCS}

# C++ sources
${DIR0003}/%.o: ${DIR0003}/%.cpp
	@echo Compiling "${GREEN}$@${NONE}"
	${QUIET}${CXX} -o $@ -c $< ${DEPFLAGS} ${${DIR0003}CXXFLAGS} ${${DIR0003}INCS}
${DIR0003}/%.s: ${DIR0003}/%.cpp
	@echo Compiling "${CYAN}$@${NONE}"
	${QUIET}${CXX} -o $@ $< ${ASMFLAGS} ${${DIR0003}CXXFLAGS} ${${DIR0003}INCS}

# Linking pattern rule for this directory
%.exe: ${DIR0003}/%.o
	@echo Linking "${RED}$@${NONE}"
	${QUIET}${CXX} -o $@ $^ ${LIBS}
endif 
