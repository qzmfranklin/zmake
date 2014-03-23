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
-include root.mk# start with root
################################################################################
#	ADD BRANCHES HERE
-include heapsort/branch.mk
-include diffpar/branch.mk
################################################################################ 
-include ${DEP}# include all .d files
################################################################################ 
#	LIST LINKING RULES
diffpar.exe: diffpar/diffpar.o
	@echo "Linking ${RED}$@${NONE}..."
	${QUIET}${CXX} -o $@ $^ ${LIBS} 

test_heapsort.exe: heapsort/heapsort.o heapsort/test_heapsort.o
	@echo "Linking ${RED}$@${NONE}..."
	${QUIET}${CXX} -o $@ $^ ${LIBS}
################################################################################
#	MISC TARGETS 
.PHONY: all 				\
	clean cleanx cleanxx 		\
	help 				\
	dist

all: ${OBJ}
	@echo "${GREEN}Finished compiling.${NONE}"
clean:
	${QUIET}rm -f ${OBJ}
cleanx:
	${QUIET}rm -f ${DEP} ${ASM}
cleanxx: clean
	${QUIET}rm -f *.exe
